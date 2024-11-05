#include "apu.h"
#include "../include/miniaudio.h"

ma_waveform sineWave;
ma_device_config deviceConfig;
ma_device device;
ma_waveform_config sineWaveConfig;


void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    ma_waveform* pSineWave = (ma_waveform*)pDevice->pUserData;
    //ma_waveform_read_pcm_frames(pSineWave, pOutput, frameCount, NULL);
}

void APUInit() {
    //deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format   = ma_format_f32;
    deviceConfig.playback.channels = 2;
    deviceConfig.sampleRate        = 48000;
    deviceConfig.dataCallback      = data_callback;
    deviceConfig.pUserData         = &sineWave;

    //ma_device_init(NULL, &deviceConfig, &device);
}


uint8_t APURead(uint16_t index) {
    if (index == NR10) return 0x80 + (CH1Pace << 4) + (CH1Direction << 3) + CH1Step;
    if (index == NR11) return (CH1WaveDuty << 6) + 0x3F;
    if (index == NR12) return (CH1InitVolume << 4) + (CH1EnvDir << 3) + CH1SweepPace;
    if (index == NR14) return (CH1LengthEnable << 6) + 0xDF;
    if (index == NR21) return (CH2WaveDuty << 6) + 0x3F;
    if (index == NR22) return (CH2InitVolume << 4) + (CH2EnvDir << 3) + CH2SweepPace;
    if (index == NR24) return (CH2LengthEnable << 6) + 0xDF;
    return 0;
}


void APUWrite(uint16_t index, uint8_t value) {

}


void APUTick(int cycles) {

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
