#include "bus.h"
#include "joypad.h"

uint8_t read(uint16_t index) {
    if (index < 0x0100 && io_read(rBOOT)) return boot_rom[index];
    else if (index < 0x4000) return rom_bank_0  [index-0x0000];
    else if (index < 0x8000) return rom_bank_1  [index-0x4000];
    else if (index < 0xA000) return vram        [index-0x8000];
    else if (index < 0xC000) return ext_ram     [index-0xA000];
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
    else if (index < 0x4000) {}
    else if (index < 0x8000) {}
    else if (index < 0xA000) vram        [index-0x8000] = value;
    else if (index < 0xC000) ext_ram     [index-0xA000] = value;
    else if (index < 0xD000) wram_bank_0 [index-0xC000] = value;
    else if (index < 0xE000) wram_bank_1 [index-0xD000] = value;
    else if (index < 0xE000) wram_bank_1 [index-0xD000] = value;
    else if (index < 0xFE00) {}
    else if (index < 0xFEA0) OAM         [index-0xFE00] = value;
    else if (index < 0xFF00) {}
    else if (index < 0xFF80) io_write(index, value);
    else if (index < 0xFFFF) HRAM        [index-0xFF80] = value;
    else IE = value;

}


uint8_t io_read(enum IO io) {
    if (io == rJOY) return joypad_read();
    return io_regs[io-0xff00];
}


void io_write(enum IO io, uint8_t value) {
    if (io == rJOY) joypad_write(value);
    io_regs[io-0xff00] = value;
}


void open_bootrom_file(char* p) {
    FILE *f = fopen(p, "rb");
    if (f == NULL) {
        printf("Boot Rom (%s) Not found!\n", p);
        exit(-1);
    }
    for (int i = 0; i < 0x100; i++) boot_rom[i] = fgetc(f);
}


void open_cartridge_file(char* p) {
    FILE *f = fopen(p, "rb");
    if (f == NULL) {
        printf("Cartridge Rom (%s) Not found!\n", p);
        exit(-1);
    }
    for (int i = 0; i < 0x4000; i++) rom_bank_0[i] = fgetc(f);
    for (int i = 0; i < 0x8000; i++) rom_bank_1[i] = fgetc(f);
}


