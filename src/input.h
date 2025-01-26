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

#ifndef INPUT_H
#define INPUT_H

/* There exists no input.c file in this directory. Instead these prototypes are implemented by pi/inputPi.c
 * and Desktop/inputDesktop.c, depending on what platform is used. The system works by being given a callback 
 * function upstream, which is then called when one of the BUTTON buttons are pressed */

enum BUTTON {BUTTON_A, BUTTON_B, BUTTON_START, BUTTON_SELECT, BUTTON_UP, BUTTON_DOWN, BUTTON_LEFT, BUTTON_RIGHT, BUTTON_SPECIAL};

enum PRESS_TYPE {RELEASE, PRESS};



void inputSetCallback(void (*callback)(enum BUTTON, enum PRESS_TYPE));

void inputInit();
void inputTick(void* null); // PASS NULL TO THIS

void inputClearCallback();



#endif
