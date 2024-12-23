#include "timer.h"
#include "interrupts.h"


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
    clock_freq = 9;
}


int previous_timer_counter = 0;
bool previous_timer_edge = 0;

uint8_t clock_freq = 0x10;
uint16_t clock_and_up = 0xf0;
uint8_t clock_freq_index = 10;
void timerTick(int cycles) {
    if (!timer_enabled) {
        sys_counter += cycles;
        return;
    }
    if ((sys_counter & clock_and_up) != ((sys_counter + cycles) & clock_and_up)) {
        int prev = timer_counter;
        timer_counter += (((sys_counter & ~clock_and_up) + cycles) >> clock_freq_index);
        if (timer_counter < prev) {
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
        clock_freq = 1 << (clock_select == 0 ? 9 : (clock_select * 2 + 1));
        clock_and_up = 0xffff << (clock_select == 0 ? 10 : (clock_select * 2 + 2));
        clock_freq_index = (clock_select == 0 ? 10 : (clock_select * 2 + 2));
        timer_enabled = value & 4;
    }
}

