/*
 * Copyright (C) 2025 Aaron Kainu
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/



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
