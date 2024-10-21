#include "gameboy.h"
#include "cpu.h"
#include "interrupts.h"
#include "ppu.h"


void GameboyInit() {
    z = 7;
    n = 6;
    hy = 5;
    cy = 4;
    is_interrupting = false;
    IME = false;

    init_ppu();
}

int GameboyCartridgeLoad() {
    for (int i = 0; i < 65536;i++) {
        data[i] = 0;
    }


    FILE *boot = fopen("demoFiles/dmg_boot.bin", "rb");
    if (boot == NULL) {
        printf("Boot File Not Found!\n");
        return 0;
    }
    int i = 0;
    while (i < 0x100) {
        boot_data[i++] = fgetc(boot);
    }
    fclose(boot);
    

    FILE *file = fopen("demoFiles/Tetris.gb", "rb");
    if (file == NULL) {
        printf("Game File Not Found!\n");
        return 0;
    }
    i = 0;
    while (!feof(file)) {
        data[i++] = fgetc(file);
    }
    fclose(file);

    return 1;
}

void *GameboyThreadLoop() {
    PC = 0x00;
    serial_interrupt = 0;

    double speed = 1.0f / 4194304.0f;
    
    int last_div = 0;

    int last_timer = 0;
    int timer_speeds[4] = {256, 4, 16, 64};


    int interrupt_called = 0;
    uint8_t previous_tima = 0;
    
    while (true) {
        clock_t begin = clock();
        int cycle_length = 4;

        if (halt == NOHALT) {
            if (interrupt_called) interrupt(&interrupt_called);
            cycle_length = execute_op();
            
            f &= 0xf0;
        
        } else if (interrupt_called) {
            if (IME) interrupt(&interrupt_called);
            else if (halt == HALTNOIMENOINT) halt = NOHALT;
            else printf("Halt Bug isn't bugging?\n");
        }


        // Timer and Divider Registers
        last_div += cycle_length;
        if (last_div >= 256) {
            data[0xff04]++;
            last_div %= 256;
        }
        if (data[0xff07] & 4) { // Timer
            last_timer+=(int)(cycle_length/4); // erm wat da sigma
            //printf("%i\n", last_timer);
            if (last_timer >= timer_speeds[data[0xff07] % 4]) {
                data[0xff05] += (int)(last_timer / timer_speeds[data[0xff07]%4]);
                last_timer %= timer_speeds[data[0xff07] % 4];
                if (data[0xff05] < previous_tima) { // OVERFLOW
                    data[0xff05] = data[0xff06];
                    data[0xff0f] = data[0xff0f] | 4;
                    request_interrupt(TIMER);
                }
                previous_tima = data[0xff05];
            }
        }


        // Serial Interrupt
        if (serial_interrupt != 0) {
            serial_interrupt = 0;
            request_interrupt(SERIAL);
        }

        /*                  LCD                  */
        LCDTick(cycle_length);



        // Unofficial Interrupt
        check_interrupts(&interrupt_called);
        

        if (PC > 0x0) {
            //printf("A:%.2X F:%.2X B:%.2X C:%.2X D:%.2X E:%.2X H:%.2X L:%.2X SP:%.4X PC:%.4X PCMEM:%.2X,%.2X,%.2X,%.2X LY:%.2X\n",a, f, b, c, d, e, h, l, SP, PC, fetch(PC), fetch(PC+1), fetch(PC+2), fetch(PC+3), fetch(0xff44));
        }



        // Cycle Timing
        double time = 0;
        do time = ((double)(clock() - begin) / CLOCKS_PER_SEC);
        while (time < speed * cycle_length);
        
    }
    printf("--- DONE ---\n");

}


void GraphicsThreadLoop() {
    int previous_mode = 2;
    while (true) {
        if (!(data[0xff40] & 128)) continue;
        if (previous_mode != (data[0xff41] & 3)) {
            previous_mode = data[0xff41] & 3;
            if (previous_mode == 0) {
                draw_scanline(data[0xff44]);
            } else if (previous_mode == 1) {
                if (!render_frame()) break;
            }
        }
    }
}
