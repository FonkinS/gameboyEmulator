#include "bus.h"
#include "lcd.h"
#include "mbc.h"

 bool bootrom_enabled;

 uint8_t boot_rom[0x100];    // 0x0000-0x0100
 uint8_t vram[0x2000];       // 0x8000-0x9FFF
 uint8_t wram_bank_0[0x1000];// 0xC000-0xCFFF
 uint8_t wram_bank_1[0x1000];// 0xD000-0xDFFF
 uint8_t OAM[0xA0];          // 0xFE00-0xFE9F
 uint8_t io_regs[0x80];      // 0xFF00-0xFF7F
 uint8_t HRAM[0x7F];         // 0xFF80-0xFFFE



uint8_t BusRead(uint16_t index) {
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


void BusWrite(uint16_t index, uint8_t value) {
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
    if (io == NR10) return APURead(io);
    if (io == NR11) return APURead(io);
    if (io == NR12) return APURead(io);
    if (io == NR13) return APURead(io);
    if (io == NR14) return APURead(io);
    if (io == NR21) return APURead(io);
    if (io == NR22) return APURead(io);
    if (io == NR23) return APURead(io);
    if (io == NR24) return APURead(io);
    if (io == NR30) return APURead(io);
    if (io == NR31) return APURead(io);
    if (io == NR32) return APURead(io);
    if (io == NR33) return APURead(io);
    if (io == NR34) return APURead(io);
    if (io == NR41) return APURead(io);
    if (io == NR42) return APURead(io);
    if (io == NR43) return APURead(io);
    if (io == NR44) return APURead(io);
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
    else if (io == NR10) APUWrite(io, value);
    else if (io == NR11) APUWrite(io, value);
    else if (io == NR12) APUWrite(io, value);
    else if (io == NR13) APUWrite(io, value);
    else if (io == NR14) APUWrite(io, value);
    else if (io == NR21) APUWrite(io, value);
    else if (io == NR22) APUWrite(io, value);
    else if (io == NR23) APUWrite(io, value);
    else if (io == NR24) APUWrite(io, value);
    else if (io == NR30) APUWrite(io, value);
    else if (io == NR31) APUWrite(io, value);
    else if (io == NR32) APUWrite(io, value);
    else if (io == NR33) APUWrite(io, value);
    else if (io == NR34) APUWrite(io, value);
    else if (io == NR41) APUWrite(io, value);
    else if (io == NR42) APUWrite(io, value);
    else if (io == NR43) APUWrite(io, value);
    else if (io == NR44) APUWrite(io, value);
    else if (io == rLCDC) LCDWrite(io, value);
    else if (io == rSTAT) LCDWrite(io, value);
    else if (io == rSCY) LCDWrite(io, value);
    else if (io == rSCX) LCDWrite(io, value);
    else if (io == rLY) LCDWrite(io, value);
    else if (io == rLYC) LCDWrite(io, value);
    else if (io == rDMA) { // Pretend 640 Clock Cycles passes
        for (int i = 0; i < 0xa0; i++) {
            BusWrite(0xfe00+i, BusRead((value<<8)+i));
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


