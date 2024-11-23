#ifndef TIMER_H
#define TIMER_H
#include <stdint.h>
#include <stdbool.h>

enum TimerIO {
    rDIV = 0xff04,
    rTIMA = 0xff05,
    rTMA = 0xff06,
    rTAC = 0xff07
};

void timerInit();
void timerTick(int cycles);
uint8_t timerRead(uint16_t index);
void timerWrite(uint16_t index, uint8_t value);

extern bool timer_enabled;
extern int clock_select;

extern uint16_t sys_counter;

extern uint8_t timer_modulo;
extern uint8_t timer_counter;

extern uint8_t clock_freq;




#endif // TIMER_H
