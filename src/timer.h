#ifndef TIMER_H
#define TIMER_H
#include <stdint.h>
#include <stdbool.h>

void timerInit();
void timerTick(int cycles);
uint8_t timerRead(uint16_t index);
void timerWrite(uint16_t index, uint8_t value);

bool timer_enabled;
int clock_select;

uint16_t sys_counter;

uint8_t timer_modulo;
uint8_t timer_counter;

bool previous_timer_edge;




#endif // TIMER_H
