#include "timer.h"
#include "cpu.h"
#include "interrupts.h"
#include <time.h>


bool timer_enabled;
int clock_select;

uint16_t sys_counter;

uint8_t timer_modulo;
uint8_t timer_counter;

bool previous_timer_edge;



void timerInit() {
    timer_enabled = false;
    clock_select = 0;
    sys_counter = 0;
    timer_modulo = 0;
    timer_counter = 0;
    previous_timer_edge = false;
}


// TODO Optimize
void timerTick(int cycles) {
    for (int i = 0; i < cycles; i++) {
        sys_counter += 1; 
        bool andresult = (sys_counter & (1 << (clock_select == 0 ? 9 : (1 + clock_select * 2)))) && timer_enabled;
        if (previous_timer_edge == 1 && andresult == 0) {
            timer_counter++;
            if (timer_counter == 0) { // OVERFLOW
                timer_counter = timer_modulo;
                request_interrupt(TIMER);
            }
        }
        previous_timer_edge = andresult;
    }
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
