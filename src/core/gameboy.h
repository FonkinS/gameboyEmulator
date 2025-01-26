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

#ifndef GAMEBOY_H
#define GAMEBOY_H
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include "cpu.h"
#include "lcd.h"
#include "ppu.h"
#include "bus.h"
#include "timer.h"
#include "interrupts.h"
#include "joypad.h"


#define FRAME_DURATION 0.0167427062988f


int GameboyInit(char* boot, char* cart, void (*endcallback)());

int GameboyProcessInstruction();
void GameboyProcessFrame();

void GameboyKill();



#endif


