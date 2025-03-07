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

#include "../input.h"
#include <wiringPi.h>
#include <stdbool.h>

/* This script is exlusively for managing input on the Raspberry Pi through GPIO */

#define PI_A 14
#define PI_B 15
#define PI_START 0
#define PI_SELECT 5
#define PI_UP 6
#define PI_DOWN 13
#define PI_LEFT 19
#define PI_RIGHT 26
#define PI_SPECIAL 3

// Is set in the app/emulator to let the app/emulator handle inputs
void (*callback)(enum BUTTON, enum PRESS_TYPE);
bool callback_set = false;

bool prev_a = false;
bool prev_b = false;
bool prev_start = false;
bool prev_select = false;
bool prev_up = false;
bool prev_down = false;
bool prev_left = false;
bool prev_right = false;
bool prev_special = false;

void inputInit() {
	wiringPiSetupGpio();
	pinMode(PI_A, INPUT);
	pinMode(PI_B, INPUT);
	pinMode(PI_START, INPUT);
	pinMode(PI_SELECT, INPUT);
	pinMode(PI_UP, INPUT);
	pinMode(PI_DOWN, INPUT);
	pinMode(PI_LEFT, INPUT);
	pinMode(PI_RIGHT, INPUT);
	pinMode(PI_SPECIAL, INPUT);
	pullUpDnControl(PI_A, PUD_UP);
	pullUpDnControl(PI_B, PUD_UP);
	pullUpDnControl(PI_START, PUD_UP);
	pullUpDnControl(PI_SELECT, PUD_UP);
	pullUpDnControl(PI_UP, PUD_UP);
	pullUpDnControl(PI_DOWN, PUD_UP);
	pullUpDnControl(PI_LEFT, PUD_UP);
	pullUpDnControl(PI_RIGHT, PUD_UP);
	pullUpDnControl(PI_SPECIAL, PUD_UP);
}


void inputSetCallback(void (*c)(enum BUTTON, enum PRESS_TYPE)) {
    callback_set = true;
    callback = c;
}

// If the Pi reads an input as set to LOW, it is currently being pressed
void inputTick(void* null) {
    if (callback_set) {
        if ((digitalRead(PI_A) == LOW) != prev_a) {(callback)(BUTTON_A, !prev_a); prev_a = !prev_a;} 
        if ((digitalRead(PI_B) == LOW) != prev_b) {(callback)(BUTTON_B, !prev_b); prev_b = !prev_b;} 
        if ((digitalRead(PI_START) == LOW) != prev_start) {(callback)(BUTTON_START, !prev_start); prev_start = !prev_start;} 
        if ((digitalRead(PI_SELECT) == LOW) != prev_select) {(callback)(BUTTON_SELECT, !prev_select); prev_select = !prev_select;} 
        if ((digitalRead(PI_UP) == LOW) != prev_up) {(callback)(BUTTON_UP, !prev_up); prev_up = !prev_up;} 
        if ((digitalRead(PI_DOWN) == LOW) != prev_down) {(callback)(BUTTON_DOWN, !prev_down); prev_down = !prev_down;} 
        if ((digitalRead(PI_LEFT) == LOW) != prev_left) {(callback)(BUTTON_LEFT, !prev_left); prev_left = !prev_left;} 
        if ((digitalRead(PI_RIGHT) == LOW) != prev_right) {(callback)(BUTTON_RIGHT, !prev_right); prev_right = !prev_right;} 
        if ((digitalRead(PI_SPECIAL) == LOW) != prev_special) {(callback)(BUTTON_SPECIAL, !prev_special); prev_special = !prev_special;} 
    }
}


void inputClearCallback() {
    callback_set = false;
}
