#include "gameboy.h"
#include "apu.h"
#include "interrupts.h"
#include "lcd.h"
#include "mbc.h"
#include "ppu.h"
#include <time.h>

int GameboyInit(char *boot, char *cart) {
    if (open_bootrom_file(boot)) return -1;
    if (open_cartridge_file(cart)) return -1;

    char *title = (char*) calloc(0x10, sizeof(char)); 
    for (int c = 0x134;c<0x143;c++) title[c-0x134] = BusRead(c);
    timerInit();
    CPUInit();
    APUInit();
    PPUInit(title);
    
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
    timerTick(cycle_length);
    LCDTick(cycle_length);
    APUTick(cycle_length);
    check_interrupts();

    return cycle_length;

}

// TODO Correct OAM TIming

bool GameboyProcessFrame() {
    clock_t begin = clock();
    // Non-VBLANK
    while (PPUMode != M_VBLANK) {
        while (PPUMode != M_HBLANK) GameboyProcessInstruction();
        if (LCDEnable) drawScanline(LY);
        while (PPUMode == M_HBLANK) GameboyProcessInstruction();
    }

    int out = renderFrame();

    while (PPUMode == VBLANK) GameboyProcessInstruction();

    while ((double)(clock() - begin) / CLOCKS_PER_SEC < FRAME_DURATION) {}
    //printf("%i%%\n", (int)(CLOCKS_PER_SEC / (double)(clock() - begin) / (59.726453f) * 100));

    return out;
}


void GameboyKill() {
    PPUKill();
    MBCKill();
    APUKill();
}
