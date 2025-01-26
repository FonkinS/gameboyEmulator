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

#ifndef APU_H
#define APU_H
#include <stdint.h>
#include <stdbool.h>
#include "timer.h"

void APUInit();

uint8_t APURead(uint16_t index);
void APUWrite(uint16_t index, uint8_t value);

void APUTick(int cycles);

void APUKill();

void triggerChannel(int n);
void channelTick(int cycles, int *timer, uint8_t* dutyIndex, int period);
// TODO SUper Mario Land running slowlu?
// TODO SUper Mario Land OAM Glitches
// TODO Super Mario Land Pause Screen Glitch
// TODO Save Data (Ext RAM)
// TODO POkemon Blue Main Menu issue
// TODO Pokemon Blue Music sounds shit
// TODO CH3 and CH4

enum APUIO {
    NR10 = 0xff10, // Channel 1 Sweep
    NR11 = 0xff11, // Channel 1 Length
    NR12 = 0xff12, // Channel 1 Volume
    NR13 = 0xff13, // Channel 1 Period Low
    NR14 = 0xff14, // Channel 1 Period High
    NR20 = 0xff15, 
    NR21 = 0xff16, // Channel 2 Length
    NR22 = 0xff17, // Channel 2 Volume
    NR23 = 0xff18, // Channel 2 Period Low
    NR24 = 0xff19, // Channel 2 Period High
    NR30 = 0xff1a, // Channel 3 DAC
    NR31 = 0xff1b, // Channel 3 Length
    NR32 = 0xff1c, // Channel 3 Volume
    NR33 = 0xff1d, // Channel 3 Period Low
    NR34 = 0xff1e, // Channel 3 Period High
    NR41 = 0xff20, // Channel 4 Length
    NR42 = 0xff21, // Channel 4 Volume
    NR43 = 0xff22, // Channel 4 Frequency / Randomness
    NR44 = 0xff23, // Channel 4 Control
    NR50 = 0xff24, // Master Volume
    NR51 = 0xff25, // Sound Panning 
    NR52 = 0xff26, // Master Control
};

extern bool APUEnabled;
enum SOUND_PANNING {CENTER, LEFT, RIGHT};


#endif // AUDIO_H
