#include "apu.h"
#define MINIAUDIO_IMPLEMENTATION
#include "../include/miniaudio.h"

uint8_t wave_duty[4] = {
    0x1, // 0b00000001 - 12.5%
    0x81,// 0b10000001 - 25%
    0x87,// 0b10000111 - 50%
    0x7e,// 0b01111110 - 75%
};
uint8_t CH2_wave_index = 0;

uint8_t outBuffer[4800];
uint8_t gbBuffer[4800];

int endOfOutBuffer = 0;
int gbBufferPos = 0;
//int outBufferCursor = 0;


struct Channel {
    ma_device_config deviceConfig;
    ma_device device;
};

struct Channel CH1;
struct Channel CH2;

int k = 0;
int start = 0;


void queue_audio(uint8_t* data) {
    for (int i = 0; i < 960;i++) {
        outBuffer[i+endOfOutBuffer] = data[i];
    }
    endOfOutBuffer += 960;
}


void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    uint8_t* pFramesOutF32 = (uint8_t*)pOutput;
    /*for (int i = 0; i < CLIP_LENGTH; i++) {
        printf("%i,", current_clip[i]);
    }
    printf("\n\n\n\n\n\n");*/
    //clip_index = 0;
    /*for (int i = 0; i < start+frameCount; i++) {
        printf("%i ", current_clip[i]);
    }
    printf("\n\n\n\n");*/
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
    CH2.deviceConfig = ma_device_config_init(ma_device_type_playback);
    CH2.deviceConfig.playback.format   = ma_format_u8;
    CH2.deviceConfig.playback.channels = 2;
    CH2.deviceConfig.sampleRate        = 48000;
    CH2.deviceConfig.dataCallback      = data_callback;

    ma_device_init(NULL, &CH2.deviceConfig, &CH2.device);
    ma_device_start(&CH2.device);

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
    if (index == NR13) CH1Period = (CH1Period & 0x700) + value;
    else if (index == NR14) {
        if (value & 0x80) triggerChannel(1);
        CH1Period = (CH1Period & 0xff) + ((value & 7) << 8); 
    }
    else if (index == NR21) {
        CH2WaveDuty = (value & 0xc0) >> 6;
        CH2InitLength = value & 0x3f;
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
    //if (!APUEnabled) return;
    if (CH2Enabled) {
        CH2Timer += cycles;
        if (CH2Timer > 0x7ff) {
            CH2Timer = CH2Period + (CH2Timer % 0x800);
            CH2_wave_index = (CH2_wave_index+1) % 8;
        }
    }

    
    appending_timer += cycles;
    if (appending_timer >= 75 && endOfOutBuffer < 1920) {
        appending_timer = 0;
        gbBuffer[gbBufferPos++] = ((wave_duty[CH2WaveDuty] & (1 << CH2_wave_index)) >> CH2_wave_index);
        if (gbBufferPos >= 960) {
            queue_audio(gbBuffer);
            gbBufferPos = 0;
        }
    }
    /*appending_timer += cycles;
    if (appending_timer >= 83) { // 2^22 / 48000
        appending_timer = 0;
        current_clip[clip_index++] = k++ % 220 < 110 ? 1 : 0;
        //current_clip[clip_index++] = ((wave_duty[CH2WaveDuty] & (1 << CH1_wave_index)) >> CH1_wave_index);
        if (clip_index > 24000) clip_index = 10000;
    }*/
}


void APUKill() {
    ma_device_uninit(&CH2.device);
}

/*MA_API ma_result my_read_pcm_frames(void* pFramesOut, ma_uint64 frameCount, ma_uint64* pFramesRead) {
    int frames = 48000 / 220;
    uint8_t* pFramesOutF32 = (uint8_t*)pFramesOut;
    for (ma_uint64 iFrame = 0; iFrame < frameCount; iFrame += 1) {
        uint8_t r = (k++ % frames < (frames / 2)) ? 50 : 0;
        pFramesOutF32[iFrame*2] = r;
        pFramesOutF32[iFrame*2+1] = r;
    }
    return MA_SUCCESS;
}

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    my_read_pcm_frames(pOutput, frameCount, NULL);
}

int main() {
    ma_device_config deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format   = ma_format_u8;
    deviceConfig.playback.channels = 2;
    deviceConfig.sampleRate        = 48000;
    deviceConfig.dataCallback      = data_callback;

    ma_device device;
    
    ma_device_init(NULL, &deviceConfig, &device);

    ma_device_start(&device);

    clock_t begin = clock();
    while ((double)(clock() - begin) / CLOCKS_PER_SEC < 1.0) {}

    ma_device_uninit(&device);
    
    return 0;
}*/


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



/*
 

#include "apu.h"
#define MINIAUDIO_IMPLEMENTATION
#include "../include/miniaudio.h"
#define CLIP_LENGTH 4800000

uint8_t wave_duty[4] = {
    0x1, // 0b00000001 - 12.5%
    0x81,// 0b10000001 - 25%
    0x87,// 0b10000111 - 50%
    0x7e,// 0b01111110 - 75%
};
uint8_t wave_index = 0;

uint8_t appending_timer = 0;

uint8_t *front_buffer;
uint8_t *back_buffer;
int buffer_num = 0;
int clip_index = 0;


struct Channel {
    ma_device_config deviceConfig;
    ma_device device;
};

struct Channel CH1;
struct Channel CH2;

int k = 0;
int start = 0;


int total_frames = 0;
void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    uint8_t* pFramesOutF32 = (uint8_t*)pOutput;
    for (ma_uint64 iFrame = 0; iFrame < frameCount; iFrame += 1) {
        printf("IFRAME:%i\n", total_frames++);
        pFramesOutF32[iFrame*2] = front_buffer[iFrame] * 60;
        pFramesOutF32[iFrame*2+1] = front_buffer[iFrame] * 60;
    }
}

void APUInit() {
    CH2.deviceConfig = ma_device_config_init(ma_device_type_playback);
    CH2.deviceConfig.playback.format   = ma_format_u8;
    CH2.deviceConfig.playback.channels = 2;
    CH2.deviceConfig.sampleRate        = 48000;
    CH2.deviceConfig.dataCallback      = data_callback;

    front_buffer = (uint8_t*) calloc(480, sizeof(uint8_t));
    back_buffer = (uint8_t*) calloc(480, sizeof(uint8_t));

    ma_device_init(NULL, &CH2.deviceConfig, &CH2.device);
    ma_device_start(&CH2.device);
    CH2Enabled = true;
}


void triggerChannel(int n) {
    if (n == 2) {
        int16_t period = (~CH1Period & 0x7ff) + 1;
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
        printf("Gameboy: %i\n", k);
        appending_timer = 0;
        back_buffer[clip_index++] = (k++% 218) < 109 ? 1 : 0;
        if (clip_index >= 480) {
            uint8_t *tmp = back_buffer;
            back_buffer = front_buffer;
            front_buffer = tmp;
            clip_index = 0;
        }
        //current_clip[clip_index++] = ((wave_duty[CH2WaveDuty] & (1 << wave_index)) >> wave_index);
    }
}


void APUKill() {
    ma_device_uninit(&CH2.device);
}



 
 
 */

