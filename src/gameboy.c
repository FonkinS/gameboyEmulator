#include "gameboy.h"

void GameboyInit() {
    z = 7;
    n = 6;
    hy = 5;
    cy = 4;
    IME = false;

    char *title = (char*) calloc(0x10, sizeof(char)); 
    for (int c = 0x134;c<0x143;c++) title[c-0x134] = read(c);
    init_ppu(title);
    timerInit();
}

int GameboyCartridgeLoad(char* p) {
    open_bootrom_file("demoFiles/dmg_boot.bin");
    open_cartridge_file(p);
    return 1;
}

void *GameboyThreadLoop() {
    PC = 0x00;
    serial_interrupt = 0;

    double speed = 1.0f / 4194304.0f;
    
    int last_div = 0;

    int last_timer = 0;
    int timer_speeds[4] = {256, 4, 16, 64};

    uint8_t previous_tima = 0;

    halt = NOHALT;

    while (true) {
        clock_t begin = clock();
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


        //printf("A:%.2X F:%.2X B:%.2X C:%.2X D:%.2X E:%.2X H:%.2X L:%.2X SP:%.4X PC:%.4X PCMEM:%.2X,%.2X,%.2X,%.2X – DIV:%X TIMA:%X TMA:%X TAC:%X\n",a,f,b,c,d,e,h,l,SP,PC,read(PC),read(PC+1),read(PC+2),read(PC+3),io_read(rDIV),io_read(rTIMA),io_read(rTMA),io_read(rTAC));
        //if (data[0x89b0] != 0x1b) printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa\n");
        for (int i = 0; i < 40; i++) {
            //printf("Y: %X X:%X Tile:%X Flag:%X\n", read(0))
        }
        
        // Cycle Timing
        double time = 0;
        do time = ((double)(clock() - begin) / CLOCKS_PER_SEC);
        while (time < speed * cycle_length);
        
    }
}


void GraphicsThreadLoop() {
    int previous_mode = 2;
    while (true) {
        if (!(io_read(rLCDC) & 128)) continue;
        if (previous_mode != (io_read(rSTAT) & 3)) {
            previous_mode = io_read(rSTAT) & 3;
            if (previous_mode == 0) {
                draw_scanline(io_read(rLY));
            } else if (previous_mode == 1) {
                if (!render_frame()) break;
            }
        }
    }
}
