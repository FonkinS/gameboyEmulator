#include "timer.h"
#include "cpu.h"
#include "interrupts.h"
#include <time.h>


bool timer_enabled;
int clock_select;

uint16_t sys_counter;

uint8_t timer_modulo;
uint8_t timer_counter;

uint8_t clock_freq;


void timerInit() {
    timer_enabled = false;
    clock_select = 0;
    sys_counter = 0;
    timer_modulo = 0;
    timer_counter = 0;
    clock_freq = 0;
}


int previous_timer_counter = 0;
void timerTick(int cycles) {
    if ((sys_counter / clock_freq < ((int)sys_counter + cycles) / clock_freq) && timer_enabled) {
        timer_counter += (cycles / clock_freq) + 1;
        if (timer_counter < previous_timer_counter) { // OVERFLOW
            timer_counter = timer_modulo;
            request_interrupt(TIMER);
        }
    }
    sys_counter += cycles; 
}


uint8_t timerRead(uint16_t index) {
    if (index == 0xff04) return (uint8_t)(sys_counter >> 8);
    else if (index == 0xff05) return timer_counter;
    else if (index == 0xff06) return timer_modulo;
    else if (index == 0xff07) return (timer_enabled << 2) + clock_select;
    else return 0;
}


void timerWrite(uint16_t index, uint8_t value) {
    if (index == 0xff04) sys_counter = 0;
    else if (index == 0xff05) timer_counter = value;
    else if (index == 0xff06) timer_modulo = value;
    else if (index == 0xff07) {
        clock_select = value & 3;
        clock_freq = clock_select == 0 ? 9 : (clock_select * 2 + 1);
        timer_enabled = value & 4;
    }
}


/* last_div += cycle_length;
        if (last_div >= 256) {
            io_write(rDIV, io_read(rDIV)+1);
            last_div %= 256;
        }
        if (io_read(rTAC) & 4) { // Timer
            last_timer+=(int)(cycle_length/4); // erm wat da sigma
            //printf("%i\n", last_timer);
            if (last_timer >= timer_speeds[io_read(rTAC) & 3]) {
                io_write(rTIMA,io_read(rTIMA)+(int)(last_timer / timer_speeds[io_read(rTAC)%4]));
                last_timer %= timer_speeds[io_read(rTAC) & 3];
                if (io_read(rTIMA) < previous_tima) { // OVERFLOW
                    io_write(rTIMA, io_read(rTMA));
                    request_interrupt(TIMER);
                }
                previous_tima = io_read(rTIMA);
            }
        }
*/
