#include "gameboy.h"
#include "apu.h"
#include "interrupts.h"
#include "lcd.h"
#include "mbc.h"
#include "ppu.h"
#include "joypad.h"
#include <time.h>

int GameboyInit(const char* boot, const char* cart, void (*endcallback)()) {
    if (open_bootrom_file(boot)) return -1;
    if (open_cartridge_file(cart)) return -1;
    
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
int frame_int = 0;
void GameboyProcessFrame() {
    clock_t begin = clock();
    // Non-VBLANK
    while (PPUMode != M_VBLANK) {
        while (PPUMode != M_HBLANK) GameboyProcessInstruction();
        if (LCDEnable) drawScanline(LY, 1);
        while (PPUMode == M_HBLANK) GameboyProcessInstruction();
    }

    while (PPUMode == VBLANK) GameboyProcessInstruction();

    //printf("%i FPS\n", (int)(CLOCKS_PER_SEC / (double)(clock() - begin)));
    while ((double)(clock() - begin) / CLOCKS_PER_SEC < FRAME_DURATION) {}
    //if (PC >= 0x100) return 0;
}


void GameboyKill() {
    MBCKill();
    APUKill();
}
