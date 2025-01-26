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

#include "gameboy.h"
#include "apu.h"
#include "interrupts.h"
#include "lcd.h"
#include "mbc.h"
#include "ppu.h"
#include "joypad.h"
#include <string.h>
#include <time.h>

char* save_filename;

int GameboyInit(char* boot, char* cart, void (*endcallback)()) {
    save_filename = (char*) calloc(strlen(cart)+25, sizeof(char));
    strcpy(save_filename, cart);
    strcat(save_filename, ".sav");

    if (open_bootrom_file(boot)) return -1;
    if (open_cartridge_file(cart, save_filename)) return -1;
    
    joypadInit(endcallback);
    timerInit();
    CPUInit();
    APUInit();
    return 0;
}


int GameboyProcessInstruction() {
    int cycle_length = 4;
    if (halt == NOHALT) {
        if (interrupt_called) interrupt();
        cycle_length = execute_op();
        f &= 0xf0;
    } else if (interrupt_called) {
        if (IME) interrupt();
        else if (halt == HALTNOIMENOINT) halt = NOHALT;
        else printf("Halt Bug isn't bugging?\n");
    }
    
    reset_interrupts();
    
    timerTick(cycle_length);
    LCDTick(cycle_length);
    APUTick(cycle_length);

    //if (PC >= 0x100) printf("A:%.2X F:%.2X B:%.2X C:%.2X D:%.2X E:%.2X H:%.2X L:%.2X SP:%.4X PC:%.4X PCMEM:%.2X,%.2X,%.2X,%.2X\n", a, f, b, c, d, e, h, l, SP, PC, fetch(PC), fetch(PC+1), fetch(PC+2), fetch(PC+3));

    return cycle_length;

}

// TODO Correct OAM TIming
// TODO Proper Mem timing (Read/Write happens within instruction, not at the end of it)
int frame_int = 0;
void GameboyProcessFrame() {
    clock_t begin = clock();
    // Non-VBLANK
    while (PPUMode != M_VBLANK) {
        while (PPUMode != M_HBLANK) GameboyProcessInstruction();
        if (LCDEnable) drawScanline(LY);
        while (PPUMode == M_HBLANK) GameboyProcessInstruction();
    }

    while (PPUMode == VBLANK) GameboyProcessInstruction();

    //printf("%i FPS\n", (int)(CLOCKS_PER_SEC / (double)(clock() - begin)));
    //printf("hi\n");
    while ((double)(clock() - begin) / CLOCKS_PER_SEC < FRAME_DURATION) {}
    //if (PC >= 0x100) return 0;
}


void GameboyKill() {
    MBCSaveData(save_filename);
    free(save_filename);
    MBCKill();
    APUKill();
}
