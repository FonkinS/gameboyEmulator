#ifndef BUS_H
#define BUS_H
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "joypad.h"
#include "timer.h"
#include "interrupts.h"
#include "apu.h"

#include "mbc.h"

// serial interruot
// div and timer reigsters

void interrupt();

uint8_t read(uint16_t index);
void write(uint16_t index, uint8_t value);


bool bootrom_enabled;

uint8_t boot_rom[0x100];    // 0x0000-0x0100
//uint8_t rom_bank_0[0x4000]; // 0x0000-0x3FFF
//uint8_t rom_bank_1[0x4000]; // 0x4000-0x7FFF
uint8_t vram[0x2000];       // 0x8000-0x9FFF
//uint8_t ext_ram[0x2000];    // 0xA000-0xBFFF
uint8_t wram_bank_0[0x1000];// 0xC000-0xCFFF
uint8_t wram_bank_1[0x1000];// 0xD000-0xDFFF
//      ECHO RAM               0xE000-0xFDFF
uint8_t OAM[0xA0];          // 0xFE00-0xFE9F
//      NOT USABLE             0xFEA0-0xFEFF                            
uint8_t io_regs[0x80];      // 0xFF00-0xFF7F
uint8_t HRAM[0x7F];         // 0xFF80-0xFFFE

// IO Section
enum IO {
    rSB     = 0xff01,
    rSC     = 0xff02,
    rDMA    = 0xff46,
    // TODO: Audio And Wave stuff
    rBOOT   = 0xff50
};


uint8_t io_read(int io);
void io_write(int io, uint8_t value);

// File IO
int open_bootrom_file(char* p);
int open_cartridge_file(char* p);

#endif
