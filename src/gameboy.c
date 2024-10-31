#include "gameboy.h"

int GameboyCartridgeLoad(char* p) {
    open_bootrom_file("demoFiles/dmg_boot.bin");
    open_cartridge_file(p);
    return 1;
}

void GameboyInit() {
    z = 7;
    n = 6;
    hy = 5;
    cy = 4;
    IME = false;

    char *title = (char*) calloc(0x10, sizeof(char)); 
    for (int c = 0x134;c<0x143;c++) title[c-0x134] = read(c);
    printf("Memory Bank Controller Type: $%X\n", read(0x147));
    PPUInit(title);
    timerInit();
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
    check_interrupts();

    //printf("A:%.2X F:%.2X B:%.2X C:%.2X D:%.2X E:%.2X H:%.2X L:%.2X SP:%.4X PC:%.4X PCMEM:%.2X,%.2X,%.2X,%.2X - IF:%.2X IE:%.2X IME:%s\n",a,f,b,c,d,e,h,l,SP,PC,read(PC),read(PC+1),read(PC+2),read(PC+3),IF, IE, IME ? "True" : "False");
    return cycle_length;

}


bool GameboyProcessFrame() {
    clock_t begin = clock();
    // Non-VBLANK
    for (int ly = 0; ly < 144; ly++) {
        int cycles = 0;
        while (cycles < 456) cycles += GameboyProcessInstruction();
        if (LCDEnable) drawScanline(ly);
    }
    
    // VBLANK
    int cycles = 0;
    while (cycles < 4560) cycles += GameboyProcessInstruction();

    // Wait
    while ((double)(clock() - begin) / CLOCKS_PER_SEC < FRAME_DURATION) {}

    return renderFrame();
}
