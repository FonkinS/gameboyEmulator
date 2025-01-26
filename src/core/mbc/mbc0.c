/*
 * Copyright (C) 2025 Aaron Kainu
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/



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
