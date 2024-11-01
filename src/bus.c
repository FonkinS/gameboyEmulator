#include "bus.h"
#include "lcd.h"
#include "mbc.h"

uint8_t read(uint16_t index) {
    if (index < 0x0100 && !io_read(rBOOT)) return boot_rom[index];
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


void write(uint16_t index, uint8_t value) {
    if (index < 0x0100 && io_read(rBOOT)) boot_rom[index] = value;
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


uint8_t io_read(int io) {
    uint8_t i = io_regs[io-0xff00];
    if (io == rJOY) return joypad_read();
    if (io == rDIV) return timerRead(io);
    if (io == rTIMA) return timerRead(io);
    if (io == rTMA) return timerRead(io);
    if (io == rTAC) return timerRead(io);
    if (io == rIF) return IFRead();
    if (io == rLCDC) return LCDRead(io);
    if (io == rSTAT) return LCDRead(io);
    if (io == rSCY) return LCDRead(io);
    if (io == rSCX) return LCDRead(io);
    if (io == rLY) return LCDRead(io);
    if (io == rLYC) return LCDRead(io);
    if (io == rBGP) return LCDRead(io);
    if (io == rOBP0) return LCDRead(io);
    if (io == rOBP1) return LCDRead(io);
    if (io == rWY) return LCDRead(io);
    if (io == rWX) return LCDRead(io);
    return i;
}

void io_write(int io, uint8_t value) {
    if (io == rJOY) joypad_write(value);
    else if (io == rDIV) timerWrite(io, value);
    else if (io == rTIMA) timerWrite(io, value);
    else if (io == rTMA) timerWrite(io, value);
    else if (io == rTAC) timerWrite(io, value);
    else if (io == rIF) IFWrite(value);
    else if (io == rLCDC) LCDWrite(io, value);
    else if (io == rSTAT) LCDWrite(io, value);
    else if (io == rSCY) LCDWrite(io, value);
    else if (io == rSCX) LCDWrite(io, value);
    else if (io == rLY) LCDWrite(io, value);
    else if (io == rLYC) LCDWrite(io, value);
    else if (io == rDMA) { // Pretend 640 Clock Cycles passes
        for (int i = 0; i < 0xa0; i++) {
            write(0xfe00+i, read((value<<8)+i));
        }
    }else if(io == rBGP) LCDWrite(io, value);
    else if (io == rOBP0) LCDWrite(io, value);
    else if (io == rOBP1) LCDWrite(io, value);
    else if (io == rWY) LCDWrite(io, value);
    else if (io == rWX) LCDWrite(io, value);
    else io_regs[io-0xff00] = value;
}


int open_bootrom_file(char* p) {
    FILE *f = fopen(p, "rb");
    if (f == NULL) {
        printf("Boot Rom (%s) Not found!\n", p);
        return -1;
    }
    for (int i = 0; i < 0x100; i++) boot_rom[i] = fgetc(f);
    return 0;
}


int open_cartridge_file(char* p) {
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

    if (MBCInit(data, c)) return -1;

    free(data);
    return 0;
}


