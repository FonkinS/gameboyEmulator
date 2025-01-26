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



#include "mbc1.h"
#include <stdio.h>

bool ram_enable = 0;
uint8_t rom_bank_num = 1;
uint8_t ram_bank_num = 0;
enum BANK_MODES {ROMBANKMODE, RAMBANKMODE};
uint8_t bank_mode = 0;

uint8_t **rom;
uint8_t **ram;



void MBC1Init(uint8_t *data, long long length, bool load_data) {
    rom = (uint8_t**) malloc(0x80 * sizeof(uint8_t*));
    for (int i = 0; i < 0x80; i++) rom[i] = (uint8_t*) malloc(0x4000 * sizeof(uint8_t));
    if (!load_data) {
        ram = (uint8_t**) malloc(4 * sizeof(uint8_t*));
        for (int i = 0; i < 4; i++) ram[i] = (uint8_t*) malloc(0x2000 * sizeof(uint8_t));
    }

    int bank = 0;
    int c = 0;
    for (int i = 0; i < length && i < (0x80*0x4000); i++) {
        rom[bank][c++] = data[i];
        if (c >= 0x4000) {
            c = 0;
            bank++;
        }
    }
}


uint8_t MBC1Read(uint16_t index) {
    if (index < 0x4000) return rom[rom_bank_num & 0xe0][index & 0x3FFF];
    if (index < 0x8000) return rom[rom_bank_num][index & 0x3FFF];
    if (index < 0xC000 && ram_enable) return ram[ram_bank_num][index & 0x1FFF];
    return 0xff;
}


void MBC1Write(uint16_t index, uint8_t value) {
    if (index < 0x2000) ram_enable = (value == 0xa);
    else if (index < 0x4000) rom_bank_num = (rom_bank_num & 0xe0) + ((value & 0x1f) == 0 ? 1 : (value & 0x1f));
    else if (index < 0x6000) {
        if (bank_mode == ROMBANKMODE) rom_bank_num = (rom_bank_num & 0x9f) + ((value & 3) << 5);
        if (bank_mode == RAMBANKMODE) ram_bank_num = value & 3;
    }
    else if (index < 0x8000) bank_mode = value & 1;
    else if (index < 0xC000 && ram_enable) ram[ram_bank_num][index & 0x1FFF] = value;
}

void MBC1SaveData(char* filename) {
    FILE *f = fopen(filename, "wb");
    int bank = 0;
    int j = 0;
    for (int i = 0; i < 4 * 0x2000; i++) {
        fputc(ram[bank][j++], f);
        if (j >= 0x2000) {
            j = 0;
            bank++;
        }
    }
    fclose(f);
}



void MBC1Kill() {
    for (int i = 0; i < 0x80; i++) free(rom[i]);
    free(rom);
    for (int i = 0; i < 0x4; i++) free(ram[i]);
    free(ram);
}

void MBC1LoadData(char* filename) {
    FILE *f = fopen(filename, "rb");
    ram = (uint8_t**) malloc(0x4 * sizeof(uint8_t*));
    for (int i = 0; i < 0x4; i++) ram[i] = (uint8_t*) calloc(0x2000, sizeof(uint8_t));
    if (f == NULL) return;

    int j = 0;
    int bank = 0;
    while (!feof(f)) {
        ram[bank][j++] = fgetc(f);
        if (j >= 0x2000) {
            j = 0;
            bank++;
            if (bank >= 4) break;
        }
    }
}
