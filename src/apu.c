#include "apu.h"
#include "timer.h"
#define MINIAUDIO_IMPLEMENTATION
#include "../include/miniaudio.h"

uint8_t wave_duty[4] = {
    0x1, // 0b00000001 - 12.5%
    0x81,// 0b10000001 - 25%
    0x87,// 0b10000111 - 50%
    0x7e,// 0b01111110 - 75%
};
uint8_t CH1DutyIndex = 0;
uint8_t CH2DutyIndex = 0;

uint8_t outBuffer[4800];
uint8_t gbBuffer[4800];

int endOfOutBuffer = 0;
int gbBufferPos = 0;

int APUTimer = 0;


ma_device_config deviceConfig;
ma_device device;

void queue_audio(uint8_t* data) {
    for (int i = 0; i < 960;i++) {
        outBuffer[i+endOfOutBuffer] = data[i];
    }
    endOfOutBuffer += 960;
}


void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    uint8_t* pFramesOutF32 = (uint8_t*)pOutput;
    for (ma_uint64 iFrame = 0; iFrame < frameCount; iFrame += 1) {
        pFramesOutF32[iFrame*2] = outBuffer[iFrame] * 60;
        pFramesOutF32[iFrame*2+1] = outBuffer[iFrame] * 60;
    }
    for (int i = frameCount; i < endOfOutBuffer;i++){
        outBuffer[i-frameCount] = outBuffer[i];
    }
    endOfOutBuffer -= frameCount;
    if (endOfOutBuffer < 0) endOfOutBuffer = 0;
}

void APUInit() {
    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format   = ma_format_u8;
    deviceConfig.playback.channels = 2;
    deviceConfig.sampleRate        = 48000;
    deviceConfig.dataCallback      = data_callback;

    ma_device_init(NULL, &deviceConfig, &device);
    ma_device_start(&device);

}


void triggerChannel(int n) {
    if (n == 1) {
        CH1Enabled = true;
    }
    if (n == 2) {
        CH2Enabled = true;
    }
}


uint8_t APURead(uint16_t index) {
    if (index == NR10) return 0x80 + (CH1Pace << 4) + (CH1Direction << 3) + CH1Step;
    if (index == NR11) return (CH1WaveDuty << 6) + 0x3F;
    if (index == NR12) return (CH1InitVolume << 4) + (CH1EnvDir << 3) + CH1SweepPace;
    if (index == NR14) return (CH1LengthEnable << 6) + 0xDF;
    if (index == NR21) return (CH2WaveDuty << 6) + 0x3F;
    if (index == NR22) return (CH2InitVolume << 4) + (CH2EnvDir << 3) + CH2SweepPace;
    if (index == NR24) return (CH2LengthEnable << 6) + 0xDF;

    if (index == NR52) return 0x70 + (APUEnabled << 7) + (CH4Enabled << 3) + (CH3Enabled << 2) + (CH2Enabled << 1) + CH1Enabled;
    return 0xff;
}


void APUWrite(uint16_t index, uint8_t value) {
    if (index == NR11) {
        CH1WaveDuty = (value & 0xc0) >> 6;
        CH1InitLength = value & 0x3f;
        CH1Length = CH1InitLength;
        CH1LengthEnable = true;
    }
    else if (index == NR13) CH1Period = (CH1Period & 0x700) + value;
    else if (index == NR14) {
        if (value & 0x80) triggerChannel(1);
        CH1Period = (CH1Period & 0xff) + ((value & 7) << 8); 
    }
    else if (index == NR21) {
        CH2WaveDuty = (value & 0xc0) >> 6;
        CH2InitLength = value & 0x3f;
        CH2Length = CH2InitLength;
        CH2LengthEnable = true;
    }
    else if (index == NR23) CH2Period = (CH2Period & 0x700) + value;
    else if (index == NR24) {
        if (value & 0x80) triggerChannel(2);
        CH2Period = (CH2Period & 0xff) + ((value & 7) << 8); 
    }
    else if (index == NR52) APUEnabled = value & 0x80;
    //printf("%.4x:%.2x\n", index, value);
}


int appending_timer = 0;

void APUTick(int cycles) {
    if (CH1Enabled) channelTick(cycles, &CH1Timer, &CH1DutyIndex, CH1Period);
    if (CH2Enabled) channelTick(cycles, &CH2Timer, &CH2DutyIndex, CH2Period);

    if ((sys_counter & 0x1f) < ((sys_counter - cycles) & 0x1f)) { // DIV APU timer
        APUTimer = (APUTimer+1) % 8;
        if (APUTimer % 2) {
            if (CH1LengthEnable) {
                CH1Length--;
                if (CH1Length == 0) {
                    CH1LengthEnable = false;
                    CH1Enabled = false;
                }
            }
            if (CH2LengthEnable) {
                CH2Length--;
                if (CH2Length == 0) {
                    CH2LengthEnable = false;
                    CH2Enabled = false;
                }
            }
        }
    }
    
    appending_timer += cycles;
    if (appending_timer >= 75 && endOfOutBuffer < 1920) {
        appending_timer = 0;
        gbBuffer[gbBufferPos] = 0;
        gbBuffer[gbBufferPos] += ((wave_duty[CH1WaveDuty] & (1 << CH1DutyIndex)) >> CH1DutyIndex);
        gbBuffer[gbBufferPos++] += ((wave_duty[CH2WaveDuty] & (1 << CH2DutyIndex)) >> CH2DutyIndex);
        if (gbBufferPos >= 960) {
            queue_audio(gbBuffer);
            gbBufferPos = 0;
        }
    }
}


void channelTick(int cycles, int* timer, uint8_t* dutyIndex, int period) {
    *timer += cycles/4;
    if (*timer > 0x7ff) {
        *timer = period + (*timer % 0x800);
        *dutyIndex = (*dutyIndex+1) % 8;
    }
}


void APUKill() {
    ma_device_uninit(&device);
}
