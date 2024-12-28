#include "mbc0.h"

uint8_t *rom;
uint8_t *ram;



void MBC0Init(uint8_t *data, long long length) {
    rom = (uint8_t*) malloc(0x8000 * sizeof(uint8_t)); 
    ram = (uint8_t*) malloc(0x2000 * sizeof(uint8_t)); 

    for (long long c = 0; c < length && c < 0x8000; c++) {
        rom[c] = data[c];
    }
}


uint8_t MBC0Read(uint16_t index) {
    if (index < 0x8000) return rom[index];
    if (index < 0xC000) return ram[index-0xC000];
    return 0;
}


void MBC0Write(uint16_t index, uint8_t value) {
    if (index < 0x8000) {}
    else if (index < 0xC000) ram[index-0xC000] = value;
}


void MBC0Kill() {
    free(rom);
    free(ram);
}
