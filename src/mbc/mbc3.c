#include "mbc3.h"

bool ramTimerEnabled = false;
uint8_t romBankNum = 1;

uint8_t ramBankNum = 0;
uint8_t RTCRegSelect = 0;

enum RAMRTCSELECTED {RAM, RTC};
enum RAMRTCSELECTED ramOrRTCSelected;

uint8_t **rom;
uint8_t **ram;

time_t now;
struct tm latched_time;


void MBC3Init(uint8_t *data, long long length) {
    rom = (uint8_t**) malloc(0x80 * sizeof(uint8_t*));
    for (int i = 0; i < 0x80; i++) rom[i] = (uint8_t*) malloc(0x4000 * sizeof(uint8_t));
    ram = (uint8_t**) malloc(0x4 * sizeof(uint8_t*));
    for (int i = 0; i < 0x4; i++) ram[i] = (uint8_t*) malloc(0x2000 * sizeof(uint8_t));

    int bank = 0;
    int c = 0;
    for (long long i = 0; i < length; i++) {
        rom[bank][c++] = data[i];
        if (c >= 0x4000) {
            bank++;
            c = 0;
        }
    }
}


uint8_t MBC3Read(uint16_t index) {
    if (index < 0x4000) return rom[0][index];
    if (index < 0x8000) return rom[romBankNum][index - 0x4000];
    if (index < 0xC000 && ramTimerEnabled) return (ramOrRTCSelected == RAM ? ram[ramBankNum][index-0xa000] : MBC3GetRTC());
    return 0;
}


void MBC3Write(uint16_t index, uint8_t value) {
    if (index < 0x2000) ramTimerEnabled = (value == 0xa);
    else if (index < 0x4000) romBankNum = ((value & 0x7f) == 0 ? 1 : value & 0x7f);
    else if (index < 0x6000) {
        if (index < 0x4) {
            ramOrRTCSelected = RAM;
            ramBankNum = value;
        } else if (index < 0xd) {
            ramOrRTCSelected = RTC;
            RTCRegSelect = value;
        }
    } else if (index < 0x8000 && value == 1) {
        now = time(0);
        latched_time = *localtime(&now); 
    }
}


void MBC3Kill() {
    for (int i = 0; i < 0x80; i++) free(rom[i]);
    free(rom);
    for (int i = 0; i < 0x4; i++) free(ram[i]);
    free(ram);
}


// TODO RTC Day Counter Carry bit and RTC Halt
uint8_t MBC3GetRTC() {
    if (RTCRegSelect == 0x8) return latched_time.tm_sec;
    if (RTCRegSelect == 0x9) return latched_time.tm_min;
    if (RTCRegSelect == 0xa) return latched_time.tm_hour;
    if (RTCRegSelect == 0xb) return (latched_time.tm_yday + (latched_time.tm_year * 365)) & 0xff;
    if (RTCRegSelect == 0xc) return ((latched_time.tm_yday + (latched_time.tm_year * 365)) & 0x100) >> 8;
    return 0;
}



