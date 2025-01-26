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

#ifndef INTERRUPTS_H
#define INTERRUPTS_H
#include <stdint.h>
#include <stdbool.h>

enum INTERRUPT_TYPES {VBLANK=1, LCDSTAT=2, TIMER=4, SERIAL=8, JOYPAD=16};
enum InterruptIO {
    rIF = 0xff0f
};

extern uint8_t IF;
extern uint8_t IE;

extern bool IME;

extern int interrupt_called;

uint8_t interruptTypeToLocation(uint8_t type);


void request_interrupt(int type);
//void check_interrupts();
void reset_interrupts();

uint8_t IFRead();
void IFWrite(uint8_t value);


#endif
