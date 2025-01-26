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

#include "bus.h"
#include "joypad.h"
#include "lcd.h"
#include "mbc.h"

 bool bootrom_enabled;

 /* These are the built in ram and rom banks */
 uint8_t boot_rom[0x100];    // 0x0000-0x0100
 uint8_t vram[0x2000];       // 0x8000-0x9FFF
 uint8_t wram_bank_0[0x1000];// 0xC000-0xCFFF
 uint8_t wram_bank_1[0x1000];// 0xD000-0xDFFF
 uint8_t OAM[0xA0];          // 0xFE00-0xFE9F
 uint8_t io_regs[0x80];      // 0xFF00-0xFF7F
 uint8_t HRAM[0x7F];         // 0xFF80-0xFFFE



uint8_t BusRead(uint16_t index) {
    if (index < 0x0100 && io_read(rBOOT) == 0) return boot_rom[index];
    else if (index < 0x8000) return MBCRead(index);
    else if (index < 0xA000) return vram        [index-0x8000];
    else if (index < 0xC000) return MBCRead(index);
    else if (index < 0xD000) return wram_bank_0 [index-0xC000];
    else if (index < 0xE000) return wram_bank_1 [index-0xD000];
    else if (index < 0xE000) return wram_bank_1 [index-0xD000];
    else if (index < 0xFE00) return 0; // ECHO RAM
    else if (index < 0xFEA0) return OAM         [index-0xFE00];
    else if (index < 0xFF00) return 0xff; // Non Usable
    else if (index < 0xFF80) return io_read(index);
    else if (index < 0xFFFF) return HRAM        [index-0xFF80];
    else return IE;
}


void BusWrite(uint16_t index, uint8_t value) {
    if (index < 0x0100 && io_read(rBOOT) == 0) {}
    else if (index < 0x8000) MBCWrite(index, value);
    else if (index < 0xA000) vram        [index-0x8000] = value;
    else if (index < 0xC000) MBCWrite(index, value);
    else if (index < 0xD000) wram_bank_0 [index-0xC000] = value;
    else if (index < 0xE000) wram_bank_1 [index-0xD000] = value;
    else if (index < 0xFE00) {}
    else if (index < 0xFEA0) OAM         [index-0xFE00] = value;
    else if (index < 0xFF00) {}
    else if (index < 0xFF80) io_write(index, value);
    else if (index < 0xFFFF) HRAM        [index-0xFF80] = value;
    else IE = value;

}


/* The IO Read and Write functions forward computation to each sub-module*/
uint8_t io_read(int io) {
    uint8_t i = io_regs[io-0xff00];
    switch (io) {
        case rJOY:
            return joypadRead();
            break;
        case rDIV:
            return timerRead(io);
            break;
        case rTIMA:
            return timerRead(io);
            break;
        case rTMA:
            return timerRead(io);
            break;
        case rTAC:
            return timerRead(io);
            break;
        case rIF:
            return IFRead();
            break;
        case NR10:
            return APURead(io);
            break;
        case NR11:
            return APURead(io);
            break;
        case NR12:
            return APURead(io);
            break;
        case NR13:
            return APURead(io);
            break;
        case NR14:
            return APURead(io);
            break;
        case NR21:
            return APURead(io);
            break;
        case NR22:
            return APURead(io);
            break;
        case NR23:
            return APURead(io);
            break;
        case NR24:
            return APURead(io);
            break;
        case NR30:
            return APURead(io);
            break;
        case NR31:
            return APURead(io);
            break;
        case NR32:
            return APURead(io);
            break;
        case NR33:
            return APURead(io);
            break;
        case NR34:
            return APURead(io);
            break;
        case NR41:
            return APURead(io);
            break;
        case NR42:
            return APURead(io);
            break;
        case NR43:
            return APURead(io);
            break;
        case NR44:
            return APURead(io);
            break;
        case NR50:
            return APURead(io);
            break;
        case NR51:
            return APURead(io);
            break;
        case NR52:
            return APURead(io);
            break;
        case rLCDC:
            return LCDRead(io);
            break;
        case rSTAT:
            return LCDRead(io);
            break;
        case rSCY:
            return LCDRead(io);
            break;
        case rSCX:
            return LCDRead(io);
            break;
        case rLY:
            return LCDRead(io);
            break;
        case rLYC:
            return LCDRead(io);
            break;
        case rBGP:
            return LCDRead(io);
            break;
        case rOBP0:
            return LCDRead(io);
            break;
        case rOBP1:
            return LCDRead(io);
            break;
        case rWY:
            return LCDRead(io);
            break;
        case rWX:
            return LCDRead(io);
            break;

    }
        return i;
}

void io_write(int io, uint8_t value) {
    switch (io) {
        case rJOY:
            joypadWrite(value);
            break;
        case rDIV:
            timerWrite(io, value);
            break;
        case rTIMA:
            timerWrite(io, value);
            break;
        case rTMA:
            timerWrite(io, value);
            break;
        case rTAC:
            timerWrite(io, value);
            break;
        case rIF:
            IFWrite(value);
            break;
        case NR10:
            APUWrite(io, value);
            break;
        case NR11:
            APUWrite(io, value);
            break;
        case NR12:
            APUWrite(io, value);
            break;
        case NR13:
            APUWrite(io, value);
            break;
        case NR14:
            APUWrite(io, value);
            break;
        case NR21:
            APUWrite(io, value);
            break;
        case NR22:
            APUWrite(io, value);
            break;
        case NR23:
            APUWrite(io, value);
            break;
        case NR24:
            APUWrite(io, value);
            break;
        case NR30:
            APUWrite(io, value);
            break;
        case NR31:
            APUWrite(io, value);
            break;
        case NR32:
            APUWrite(io, value);
            break;
        case NR33:
            APUWrite(io, value);
            break;
        case NR34:
            APUWrite(io, value);
            break;
        case NR41:
            APUWrite(io, value);
            break;
        case NR42:
            APUWrite(io, value);
            break;
        case NR43:
            APUWrite(io, value);
            break;
        case NR44:
            APUWrite(io, value);
            break;
        case NR50:
            APUWrite(io, value);
            break;
        case NR51:
            APUWrite(io, value);
            break;
        case NR52:
            APUWrite(io, value);
            break;
        case rLCDC:
            LCDWrite(io, value);
            break;
        case rSTAT:
            LCDWrite(io, value);
            break;
        case rSCY:
            LCDWrite(io, value);
            break;
        case rSCX:
            LCDWrite(io, value);
            break;
        case rLY:
            LCDWrite(io, value);
            break;
        case rLYC:
            LCDWrite(io, value);
            break;
        case rDMA: // Pretend 640 Clock Cycles passes
            for (int i = 0; i < 0xa0; i++) BusWrite(0xfe00+i, BusRead((value<<8)+i));
            break;
        case rBGP:
            LCDWrite(io, value);
            break;
        case rOBP0:
            LCDWrite(io, value);
            break;
        case rOBP1:
            LCDWrite(io, value);
            break;
        case rWY:
            LCDWrite(io, value);
            break;
        case rWX:
            LCDWrite(io, value);
            break;
        default:
            io_regs[io-0xff00] = value;
            break;

    }
}


int open_bootrom_file(char* p) {
    FILE *f = fopen(p, "rb");
    if (f == NULL) {
        printf("Boot Rom (%s) Not found!\n", p);
        return -1;
    }
    for (int i = 0; i < 0x100; i++) boot_rom[i] = fgetc(f);
    // Reenable bootrom
    io_regs[0x50] = 0;
    bootrom_enabled = true;
    return 0;
}


int open_cartridge_file(char* p, char* save_filename) {
    FILE *f = fopen(p, "rb");
    if (f == NULL) {
        printf("Cartridge Rom (%s) Not found!\n", p);
        return -1;
    }
    // Read Info
    uint8_t *data = (uint8_t*) malloc(0x8000 * sizeof(uint8_t));
    long long c = 0;
    int max = 0x8000;
    while (!feof(f)) {
        data[c++] = fgetc(f);
        if (c >= max) {
            max += 0x8000;
            data = (uint8_t*) realloc(data, max * sizeof(uint8_t));
        }
    }

    if (MBCInit(data, c, save_filename)) return -1;

    free(data);
    return 0;
}


