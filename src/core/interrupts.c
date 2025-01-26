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

#include "interrupts.h"


uint8_t IF;
uint8_t IE;

bool IME;

int interrupt_called;



void request_interrupt(int type) {
    IF |= type;
    if (interrupt_called > type || interrupt_called == 0) {
        if (IE & type) interrupt_called = type;
    }
}


void reset_interrupts() {
    interrupt_called = 0;
}


uint8_t interruptTypeToLocation(uint8_t type) {
    switch (type) {
        case 1:
            return 0x40;
        case 2:
            return 0x48;
        case 4:
            return 0x50;
        case 8:
            return 0x58;
        case 16:
            return 0x60;
        default:
            return 0;
    }
}


uint8_t IFRead() {
    return 0xe0 | IF;
}


void IFWrite(uint8_t value) {
    IF = value;
}

