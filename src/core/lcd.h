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



#ifndef LCD_H
#define LCD_H
#include "interrupts.h"
#include <stdbool.h>
#include <stdio.h>

enum LCDIO {
    rLCDC = 0xff40,
    rSTAT = 0xff41,
    rSCY = 0xff42,
    rSCX = 0xff43,
    rLY = 0xff44,
    rLYC = 0xff45,
    rBGP = 0xff47,
    rOBP0 = 0xff48,
    rOBP1 = 0xff49,
    rWY = 0xff4a,
    rWX = 0xff4b
};

extern int dot;

// LCD Control
extern bool LCDEnable;
extern uint16_t WinTileMap;
extern bool WinEnable;
extern uint16_t BGWinTileData;
extern uint16_t BGTileMap;
extern uint8_t OBJSize;
extern bool OBJEnable;
extern bool BGWinEnable;

extern uint8_t LY;
extern uint8_t LYC;

// STAT
extern bool LYCInt;
extern bool M2Int;
extern bool M1Int;
extern bool M0Int;
extern int PPUMode;

// Scrolling
extern uint8_t SCY;
extern uint8_t SCX;
extern uint8_t WY;
extern uint8_t WX;

// Palettes
extern uint8_t BGP[4];
extern uint8_t OBP0[3];
extern uint8_t OBP1[3];




void LCDTick(int cycle_length);
uint8_t LCDRead(uint16_t index);
void LCDWrite(uint16_t index, uint8_t value);

enum PPUMODES {M_HBLANK, M_VBLANK, M_OAM, M_DRAW};



#endif
