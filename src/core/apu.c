#include "apu.h"
#define MINIAUDIO_IMPLEMENTATION
#include "../../include/miniaudio.h"

bool APUEnabled;
enum SOUND_PANNING CH1Pan;
enum SOUND_PANNING CH2Pan;
enum SOUND_PANNING CH3Pan;
enum SOUND_PANNING CH4Pan;
uint8_t soundPanReg;


uint8_t leftVolume;
uint8_t rightVolume;

// Channel 1
bool CH1Enabled;
int CH1Timer;
uint8_t CH1Pace;
bool CH1Direction;
uint8_t CH1Step;
uint8_t CH1WaveDuty;
uint8_t CH1InitLength;
uint8_t CH1Length;
uint8_t CH1InitVolume;
uint8_t CH1Volume;
bool CH1EnvDir;
uint8_t CH1SweepPace;
uint16_t CH1Period;
bool CH1LengthEnable;


// Channel 2
bool CH2Enabled;
int CH2Timer;
uint8_t CH2WaveDuty;
uint8_t CH2InitLength;
uint8_t CH2Length;
uint8_t CH2InitVolume;
uint8_t CH2Volume;
bool CH2EnvDir;
uint8_t CH2SweepPace;
uint16_t CH2Period;
bool CH2LengthEnable;


bool CH3Enabled;


// Channel 4
bool CH4Enabled;
int CH4Length;
int CH4InitLength;
int CH4Volume;
int CH4InitVolume;
bool CH4EnvDir;
int CH4SweepPace;
int CH4ClockShift;
bool CH4LSFRWidth;
int CH4ClockDivider;
bool CH4LengthEnable;

uint16_t LSFR;
int CH4Freq;




#ifdef __MACH__
#define SAMPLESIZE 480
#else
#define SAMPLESIZE 3600
#endif
#define BUFFERSIZE (SAMPLESIZE*6) 
#define SAMPLETHRESHOLD (SAMPLESIZE*4)
#define REQUESTSIZE (SAMPLESIZE*2)


uint8_t wave_duty[4] = {
    0x1, // 0b00000001 - 12.5%
    0x81,// 0b10000001 - 25%
    0x87,// 0b10000111 - 50%
    0x7e,// 0b01111110 - 75%
};
uint8_t CH1DutyIndex = 0;
uint8_t CH2DutyIndex = 0;

uint8_t outBuffer[BUFFERSIZE];
uint8_t gbBuffer[BUFFERSIZE];

int endOfOutBuffer = 0;
int gbBufferPos = 0;

int APUTimer = 0;


ma_device_config deviceConfig;
ma_device device;

void queue_audio(uint8_t* data) {
    for (int i = 0; i < REQUESTSIZE;i++) {
        outBuffer[i+endOfOutBuffer] = data[i];
    }
    endOfOutBuffer += REQUESTSIZE;
}


void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    uint8_t* pFramesOutF32 = (uint8_t*)pOutput;
    for (ma_uint64 iFrame = 0; iFrame < frameCount; iFrame += 1) {
        //if (outBuffer[iFrame] == 255) continue; // Silent
        pFramesOutF32[iFrame*2] = outBuffer[iFrame];
        pFramesOutF32[iFrame*2+1] = outBuffer[iFrame];
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
    if (n == 4) {
        LSFR = 0;
        CH4Enabled = true;
    }
}


uint8_t APURead(uint16_t index) {
    if (index == NR10) return 0x80 + (CH1Pace << 4) + (CH1Direction << 3) + CH1Step;
    if (index == NR11) return (CH1WaveDuty << 6) + 0x3F;
    if (index == NR12) return (CH1InitVolume << 4) + (CH1EnvDir << 3) + CH1SweepPace;
    if (index == NR14) return (CH1LengthEnable << 6) + 0xBF;
    if (index == NR21) return (CH2WaveDuty << 6) + 0x3F;
    if (index == NR22) return (CH2InitVolume << 4) + (CH2EnvDir << 3) + CH2SweepPace;
    if (index == NR24) return (CH2LengthEnable << 6) + 0xBF;
    if (index == NR42) return (CH4InitVolume << 4) + (CH4EnvDir << 3) + CH4SweepPace;
    if (index == NR43) return (CH4ClockShift << 4) + (CH4LSFRWidth << 3) + CH4ClockDivider;
    if (index == NR44) return (CH4LengthEnable << 6) + 0xBf;
    if (index == NR50) return (leftVolume << 4) + rightVolume;
    if (index == NR51) return soundPanReg;
    if (index == NR52) return 0x70 + (APUEnabled << 7) + (CH4Enabled << 3) + (CH3Enabled << 2) + (CH2Enabled << 1) + CH1Enabled;
    return 0xff;
}


void APUWrite(uint16_t index, uint8_t value) {
    if (index == NR10) {
        CH1Pace = (value & 0x70) >> 4;
        CH1Direction = (value & 0x8);
        CH1Step = (value & 0x7);
    } else if (index == NR11) {
        CH1WaveDuty = (value & 0xc0) >> 6;
        CH1InitLength = value & 0x3f;
        CH1Length = CH1InitLength;
    } else if (index == NR12) {
        CH1InitVolume = (value & 0xf0) >> 4;
        CH1Volume = CH1InitVolume;
        CH1EnvDir = value & 0x8;
        CH1SweepPace = value & 0x7;
    } else if (index == NR13) CH1Period = (CH1Period & 0x700) + value;
    else if (index == NR14) {
        if (value & 0x80) triggerChannel(1);
        CH1LengthEnable = value & 0x40;
        CH1Period = (CH1Period & 0xff) + ((value & 7) << 8); 
    }
    else if (index == NR21) {
        CH2WaveDuty = (value & 0xc0) >> 6;
        CH2InitLength = value & 0x3f;
        CH2Length = CH2InitLength;
    } else if (index == NR22) {
        CH2InitVolume = (value & 0xf0) >> 4;
        CH2Volume = CH2InitVolume;
        CH2EnvDir = value & 0x8;
        CH2SweepPace = value & 0x7;
    }
    else if (index == NR23) CH2Period = (CH2Period & 0x700) + value;
    else if (index == NR24) {
        if (value & 0x80) triggerChannel(2);
        CH2LengthEnable = value & 0x40;
        CH2Period = (CH2Period & 0xff) + ((value & 7) << 8); 
    } else if (index == NR41) CH4InitLength = value & 0x3f;
    else if (index == NR42) {
        CH4InitVolume = value >> 4;
        CH4Volume = CH4InitVolume;
        CH4EnvDir = value & 8;
        CH4SweepPace = value & 7;
    } else if (index == NR43) {
        CH4ClockShift = value >> 4;
        CH4LSFRWidth = value & 8;
        CH4ClockDivider = value & 7;
        if (CH4ClockDivider == 0) CH4ClockDivider = 1; // TODO SHOULD BE 1/2
        CH4Freq = 4 * CH4ClockDivider * (1 << CH4ClockShift);
    } else if (index == NR44) {
        if (value & 0x80) triggerChannel(4);
        CH4LengthEnable = value & 0x40;
    } else if (index == NR50) {
        leftVolume = (value >> 4) & 7; 
        rightVolume = value & 7; 
    } else if (index == NR51) {
        soundPanReg = value;
        CH1Pan = CENTER;
        CH2Pan = CENTER;
        CH3Pan = CENTER;
        CH4Pan = CENTER;
        if (value & 0x80) CH4Pan = LEFT;
        if (value & 0x40) CH3Pan = LEFT;
        if (value & 0x20) CH2Pan = LEFT;
        if (value & 0x10) CH1Pan = LEFT;
        if (value & 0x08) CH4Pan = RIGHT;
        if (value & 0x04) CH3Pan = RIGHT;
        if (value & 0x02) CH2Pan = RIGHT;
        if (value & 0x01) CH1Pan = RIGHT;
    }
    else if (index == NR52) APUEnabled = value & 0x80;
    //printf("%.4x:%.2x\n", index, value);
}


int appending_timer = 0;
int ch4_timer = 0;
int tim = 0;
// TODO Sound Panning
// TODO LeftRight Volume
// TODO CH3
// TODO CH4
void APUTick(int cycles) {
    if (CH1Enabled) channelTick(cycles, &CH1Timer, &CH1DutyIndex, CH1Period);
    if (CH2Enabled) channelTick(cycles, &CH2Timer, &CH2DutyIndex, CH2Period);

    tim += cycles;
    if (tim >= 16384) { // DIV APU timer
        tim = 0;
        //printf("%f\n", 4194304.0f / (double)(sys_counter - t));
        APUTimer = (APUTimer+1) % 8;
        if (APUTimer % 2) {
            if (CH1LengthEnable) {
                CH1Length++;
                if (CH1Length == 32) {    // NOTE Should be 64
                    CH1LengthEnable = false;
                    CH1Enabled = false;
                    CH1Length = 0;
                }
            }
            if (CH2LengthEnable) {
                CH2Length++;
                if (CH2Length == 32) { // NOTE should be 64
                    CH2LengthEnable = false;
                    CH2Enabled = false;
                    CH2Length = 0;
                }
            }
            if (CH4LengthEnable) {
                CH4Length++;
                if (CH4Length == 32) { // NOTE SHOULD BE 64
                    CH4Length = 0;
                    CH4LengthEnable = false;
                    CH4Enabled = false;
                }
            }
        } 
        if (APUTimer == 7) {
            int newCH1Vol = CH1Volume + (int8_t)CH1SweepPace * (int8_t)(CH1EnvDir ? 1 : -1);
            if (newCH1Vol >= 0 && newCH1Vol <= 15) CH1Volume = newCH1Vol;
            int newCH2Vol = CH2Volume + (int8_t)CH2SweepPace * (int8_t)(CH2EnvDir ? 1 : -1);
            if (newCH2Vol >= 0 && newCH2Vol <= 15) CH2Volume = newCH2Vol;
            int newCH4Vol = CH4Volume + (int8_t)CH4SweepPace * (int8_t)(CH4EnvDir ? 1 : -1);
            if (newCH4Vol >= 0 && newCH4Vol <= 15) CH4Volume = newCH4Vol;

        }
    }
    ch4_timer += cycles;
    if (ch4_timer >= CH4Freq) {
        ch4_timer %= CH4Freq;
        bool result = (LSFR & 1) == ((LSFR >> 1) & 1);
        LSFR >>= 1;
        LSFR |= result << 14;
        if (CH4LSFRWidth == 1) { // 7 bit mode
            LSFR &= ~128;
            LSFR |= result << 6;
        }
    }
    
    appending_timer += cycles;
    if (appending_timer >= 75 && endOfOutBuffer < SAMPLETHRESHOLD) {
        appending_timer = 0;
        gbBuffer[gbBufferPos] = 0;
        if (CH1Enabled) gbBuffer[gbBufferPos] += ((wave_duty[CH1WaveDuty] & (1 << CH1DutyIndex)) >> CH1DutyIndex) * CH1Volume;
        if (CH2Enabled) gbBuffer[gbBufferPos] += ((wave_duty[CH2WaveDuty] & (1 << CH2DutyIndex)) >> CH2DutyIndex) * CH2Volume;
        if (CH4Enabled) gbBuffer[gbBufferPos] += (LSFR & 1) * CH4Volume;
        //if (CH1Volume == 0 && CH2Volume == 0) gbBuffer[gbBufferPos] = 255;
        gbBufferPos++;
        if (gbBufferPos >= REQUESTSIZE) {
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
