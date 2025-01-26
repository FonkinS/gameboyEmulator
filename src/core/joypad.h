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



#ifndef JOYPAD_H
#define JOYPAD_H
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "../input.h"

void joypadInit(void (*endcallback)());
uint8_t joypadRead();
void joypadWrite(uint8_t value);

void joypadInputCallback(enum BUTTON button, enum PRESS_TYPE type);

enum JoypadIO {
    rJOY = 0xff00
};


extern bool button_start;
extern bool dpad_down;
extern bool button_select;
extern bool dpad_up;
extern bool button_b;
extern bool dpad_left;
extern bool button_a;
extern bool dpad_right;





#endif
