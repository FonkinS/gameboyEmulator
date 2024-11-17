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

uint8_t BusRead(uint16_t index);
void BusWrite(uint16_t index, uint8_t value);


extern bool bootrom_enabled;

extern uint8_t boot_rom[0x100];    // 0x0000-0x0100
extern uint8_t vram[0x2000];       // 0x8000-0x9FFF
extern uint8_t wram_bank_0[0x1000];// 0xC000-0xCFFF
extern uint8_t wram_bank_1[0x1000];// 0xD000-0xDFFF
extern uint8_t OAM[0xA0];          // 0xFE00-0xFE9F
extern uint8_t io_regs[0x80];      // 0xFF00-0xFF7F
extern uint8_t HRAM[0x7F];         // 0xFF80-0xFFFE

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
