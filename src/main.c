#include <stdio.h>
#include <time.h>
#include "cpu.h"

int main() {
    z = 7;
    n = 6;
    hy = 5;
    cy = 4;
    is_interrupting = false;
    IME = false;

    FILE *file = fopen("demoFiles/blarggsTest/03-op sp,hl.gb", "rb");
    if (file == NULL) {
        printf("File Not Found!\n");
        return -1;
    }
    for (int i = 0; i < 65536;i++) {
        data[i] = 0;
    }
    int i = 0;
    while (!feof(file)) {
        data[i++] = fgetc(file);
    }

    set_af(0x01b0);
    set_bc(0x0013);
    set_de(0x00d8);
    set_hl(0x014d);
    SP =  0xFFFE;
    data[0xff04] = 0xbd; // SET BACK TO ZERO WHEN INIT
    data[0xFF05] = 0x00;
    data[0xFF06] = 0x00;
    data[0xFF07] = 0x00;
    data[0xFF10] = 0x80;
    data[0xFF11] = 0xBF;
    data[0xFF12] = 0x3F;
    data[0xFF14] = 0xBF;
    data[0xFF16] = 0x3F;
    data[0xFF17] = 0x00;
    data[0xFF19] = 0xBF;
    data[0xFF1A] = 0x7F;
    data[0xFF1B] = 0xFF;
    data[0xFF1C] = 0x9F;
    data[0xFF1E] = 0xBF;
    data[0xFF20] = 0xFF;
    data[0xFF21] = 0x00;
    data[0xFF22] = 0x00;
    data[0xFF23] = 0xBF;
    data[0xFF24] = 0x77;
    data[0xFF25] = 0xF3;
    data[0xFF26] = 0xF1;
    data[0xFF40] = 0x91;
    data[0xFF42] = 0x00;
    data[0xFF43] = 0x00;
    data[0xFF45] = 0x00;
    data[0xFF47] = 0xFC;
    data[0xFF48] = 0xFF;
    data[0xFF49] = 0xFF;
    data[0xFF4A] = 0x00;
    data[0xFF4B] = 0x00;
    data[0xFFFF] = 0x00;

    PC = 0x100;
    SP = 0xfffe;
    serial_interrupt = 0;

    a = 0x01;
    f = 0xb0;
    b = 0x00;
    c = 0x13;
    d = 0x00;
    e = 0xd8;
    h = 0x01;
    l = 0x4d;

    cycles = 0;
    count = 0;


    double speed = 1.0f / 4194304.0f;
    
    int last_div = 0;

    int last_timer = 0;
    int timer_speeds[4] = {256, 4, 16, 64};

    int interrupt_next_op = 0;
    just_interrupted = false;

    uint8_t previous_tima = 0;

    while (true) {
        clock_t begin = clock();
        int cycle_length = 4;
        if (halt == NOHALT) {
            //printf("%i: IF:%X vs EF:%X TIMA:%X TAC:%X – DIV:%X – IME:%X\n", count, data[0xff0f], data[0xffff],data[0xff05], data[0xff07], data[0xff04], IME);
            //printf("%i\n", data[0xdd02]);
            count += 1;
            if (!just_interrupted) {
                printf("A:%.2X F:%.2X B:%.2X C:%.2X D:%.2X E:%.2X H:%.2X L:%.2X SP:%.4X PC:%.4X PCMEM:%.2X,%.2X,%.2X,%.2X\n", a, f, b, c, d, e, h, l, SP, PC, fetch(PC), fetch(PC+1), fetch(PC+2), fetch(PC+3));

                //check for interrupt
                if (interrupt_next_op) {
                    just_interrupted = true;
                    interrupt(interrupt_next_op);
                    interrupt_next_op = 0;
                }

                cycle_length = execute_op();
            } else {
                just_interrupted = false;
            }
            cycles += cycle_length;
            f &= 0xf0;
            
            /*if (PC == 0xC51c) {
                break;
            }*/
        } else if (interrupt_next_op) {
            if (IME) {
                just_interrupted = true;
                interrupt(interrupt_next_op);
                interrupt_next_op = 0;
            } else if (halt == HALTNOIMENOINT) {
                halt = NOHALT;
            } else {
                printf("Halt Bug isn't bugging?\n");
            }
        }


        // Timer and Divider Registers
        last_div += cycle_length;
        if (last_div >= 256) {
            data[0xff04]++;
            last_div %= 256;
        }
        if (data[0xff07] & 4) { // Timer
            last_timer+=(int)(cycle_length/4); // erm wat da sigma
            if (last_timer >= timer_speeds[data[0xff07] % 4]) {
                data[0xff05] += (int)(last_timer / timer_speeds[data[0xff07]%4]);
                last_timer %= timer_speeds[data[0xff07] % 4];
                if (data[0xff05] < previous_tima) { // OVERFLOW
                    data[0xff05] = data[0xff06];
                    data[0xff0f] = data[0xff0f] | 4;
                    interrupt_next_op = TIMER;
                }
                previous_tima = data[0xff05];
            }
        }


        // Serial Interrupt
        if (serial_interrupt != 0 && serial_interrupt <= cycles) {
            serial_interrupt = 0;
            interrupt(SERIAL);
        }

        // Unofficial Interrupt
        if (data[0xff0f] & 31 && IME) {
            if (data[0xff0f] & 1 && data[0xffff] & 1) interrupt_next_op = VBLANK;
            if (data[0xff0f] & 2 && data[0xffff] & 2) interrupt_next_op = LCDSTAT;
            if (data[0xff0f] & 4 && data[0xffff] & 4) interrupt_next_op = TIMER;
            if (data[0xff0f] & 8 && data[0xffff] & 8) interrupt_next_op = SERIAL;
            if (data[0xff0f] & 16 && data[0xffff] & 16) interrupt_next_op = JOYPAD;
        }


        // Cycle Timing
        double time = 0;
        do {
            time = ((double)(clock() - begin) / CLOCKS_PER_SEC);
        }
        while (time < speed * cycle_length);
        
    }

    fclose(file);
    return 0;
}
