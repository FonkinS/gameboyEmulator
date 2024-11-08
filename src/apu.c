#include "apu.h"
#define MINIAUDIO_IMPLEMENTATION
#include "../include/miniaudio.h"

uint8_t wave_duty[4] = {
    0x1, // 0b00000001 - 12.5%
    0x81,// 0b10000001 - 25%
    0x87,// 0b10000111 - 50%
    0x7e,// 0b01111110 - 75%
};
uint8_t wave_index = 0;

uint8_t appending_timer = 0;

uint8_t current_clip[312];
int clip_index = 0;


struct Channel {
    ma_waveform sineWave;
    ma_device_config deviceConfig;
    ma_device device;
    ma_waveform_config sineWaveConfig;
};

struct Channel CH1;
struct Channel CH2;


void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    ma_waveform* pSineWave = (ma_waveform*)pDevice->pUserData;
    ma_waveform_read_pcm_frames(pSineWave, pOutput, frameCount, NULL);
}

void APUInit() {
    CH1.deviceConfig = ma_device_config_init(ma_device_type_playback);
    CH1.deviceConfig.playback.format   = ma_format_f32;
    CH1.deviceConfig.playback.channels = 2;
    CH1.deviceConfig.sampleRate        = 48000;
    CH1.deviceConfig.dataCallback      = data_callback;
    CH1.deviceConfig.pUserData         = &CH1.sineWave;

    CH2.deviceConfig = ma_device_config_init(ma_device_type_playback);
    CH2.deviceConfig.playback.format   = ma_format_f32;
    CH2.deviceConfig.playback.channels = 2;
    CH2.deviceConfig.sampleRate        = 48000;
    CH2.deviceConfig.dataCallback      = data_callback;
    CH2.deviceConfig.pUserData         = &CH1.sineWave;

    ma_device_init(NULL, &CH1.deviceConfig, &CH1.device);
    ma_device_init(NULL, &CH2.deviceConfig, &CH2.device);
}


void triggerChannel(int n) {
    if (n == 1) {
        int16_t period = (~CH1Period & 0x7ff) + 1;
        CH1.sineWaveConfig = ma_waveform_config_init(ma_format_f32, 2, 48000, ma_waveform_type_sine, 0.2, 131072 / (2048 - period));
        ma_waveform_init(&CH1.sineWaveConfig, &CH1.sineWave);

        ma_device_start(&CH1.device);
    } else if (n == 2) {
        int16_t period = (~CH2Period & 0x7ff) + 1;
        CH2.sineWaveConfig = ma_waveform_config_init(ma_format_f32, 2, 48000, ma_waveform_type_sine, 0.2, 131072 / (2048 - period));
        ma_waveform_init(&CH2.sineWaveConfig, &CH2.sineWave);

        ma_device_start(&CH2.device);

    }
        /*CH1Running = true;
    CH1Timer = 0;*/
}


uint8_t APURead(uint16_t index) {
    if (index == NR10) return 0x80 + (CH1Pace << 4) + (CH1Direction << 3) + CH1Step;
    if (index == NR11) return (CH1WaveDuty << 6) + 0x3F;
    if (index == NR12) return (CH1InitVolume << 4) + (CH1EnvDir << 3) + CH1SweepPace;
    if (index == NR14) return (CH1LengthEnable << 6) + 0xDF;
    if (index == NR21) return (CH2WaveDuty << 6) + 0x3F;
    if (index == NR22) return (CH2InitVolume << 4) + (CH2EnvDir << 3) + CH2SweepPace;
    if (index == NR24) return (CH2LengthEnable << 6) + 0xDF;
    return 0xff;
}


void APUWrite(uint16_t index, uint8_t value) {
    if (index == NR13) CH1Period = (CH1Period & 0x700) + value;
    else if (index == NR14) {
        if (value & 0x80) triggerChannel(1);
        CH1Period = (CH1Period & 0xff) + (value & 7); 
    }
    else if (index == NR23) CH2Period = (CH2Period & 0x700) + value;
    else if (index == NR24) {
        if (value & 0x80) triggerChannel(2);
        CH2Period = (CH2Period & 0xff) + (value & 7); 
    }

}


void APUTick(int cycles) {
    if (CH2Enabled) {
        CH2Timer += cycles;
        if (CH2Timer > 0x7ff) {
            CH2Timer = CH2Period + (CH2Timer % 0x800);
            wave_index = (wave_index+1) % 8;
        }
    }

    appending_timer += cycles;
    if (appending_timer >= 87) { // 2^22 / 48000
        appending_timer = 0;
        current_clip[clip_index++] = (wave_duty[CH2WaveDuty] & (1 << wave_index)) >> wave_index;
        if (clip_index >= 312) {
            clip_index = 0;
        }
    }
}


void APUKill() {
    ma_device_uninit(&CH1.device);
}




/*int main() {
    
    sineWaveConfig = ma_waveform_config_init(ma_format_f32, 2, 48000, ma_waveform_type_sine, 0.2, 220);
    ma_waveform_init(&sineWaveConfig, &sineWave);

    ma_device_start(&device);

    for (int i = 0; i < 500; i++) {
        clock_t begin = clock();
        while ((double)(clock() - begin) / CLOCKS_PER_SEC < 0.01) {}

    }
    ma_device_uninit(&device);
    
    return 0;
}*/
