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



#include "lcd.h"


int dot;

// LCD Control
bool LCDEnable;
uint16_t WinTileMap;
bool WinEnable;
uint16_t BGWinTileData;
uint16_t BGTileMap;
uint8_t OBJSize;
bool OBJEnable;
bool BGWinEnable;

uint8_t LY;
uint8_t LYC;

// STAT
bool LYCInt;
bool M2Int;
bool M1Int;
bool M0Int;
int PPUMode;

// Scrolling
uint8_t SCY;
uint8_t SCX;
uint8_t WY;
uint8_t WX;

// Palettes
uint8_t BGP[4];
uint8_t OBP0[3];
uint8_t OBP1[3];




uint8_t LCDRead(uint16_t index) {
    if (index == rLCDC) return (LCDEnable << 7) + ((WinTileMap == 0x9c00) << 6) + (WinEnable << 5) + ((BGWinTileData == 0x8000) << 4) + ((BGTileMap == 0x9c00) << 3) + ((OBJSize == 16) << 2) + (OBJEnable << 1) + BGWinEnable;
    else if (index == rSTAT) return 0x80 + (LYCInt << 6) + (M2Int << 5) + (M1Int << 4) + (M0Int << 3) + ((LYC == LY) << 2) + PPUMode;
    else if (index == rSCY) return SCY;
    else if (index == rSCX) return SCX;
    else if (index == rLY) return LY;
    else if (index == rLYC) return LYC;
    else if (index == rBGP) return (BGP[3] << 6) + (BGP[2] << 4) + (BGP[1] << 2) + BGP[0];
    else if (index == rOBP0) return (OBP0[2] << 6) + (OBP0[1] << 4) + (OBP0[0] << 2) + 3;
    else if (index == rOBP1) return (OBP1[2] << 6) + (OBP1[1] << 4) + (OBP1[0] << 2) + 3;
    else if (index == rWY) return WY;
    else if (index == rWX) return WX;
    return 0;
}


void LCDWrite(uint16_t index, uint8_t value) {
    if (index == rLCDC) {
        LCDEnable = value & 0x80;
        if (!LCDEnable) {
            LY = 0;
            PPUMode = 0;
        }
        WinTileMap = (value & 0x40) ? 0x9c00 : 0x9800;
        WinEnable = value & 0x20;
        BGWinTileData = (value & 0x10) ? 0x8000 : 0x9000;
        BGTileMap = (value & 0x8) ? 0x9c00 : 0x9800;
        OBJSize = (value & 0x4) ? 16 : 8;
        OBJEnable = value & 2;
        BGWinEnable = value & 1;
    } else if (index == rSTAT) {
        LYCInt = value & 0x40;
        M2Int = value & 0x20;
        M1Int = value & 0x10;
        M0Int = value & 0x8;
    } else if (index == rSCY) SCY = value;
    else if (index == rSCX) SCX = value;
    else if (index == rLYC) LYC = value;
    else if (index == rSCX) SCX = value;
    else if (index == rBGP) {
        BGP[3] = (value & 0xc0) >> 6;
        BGP[2] = (value & 0x30) >> 4;
        BGP[1] = (value & 0x0c) >> 2;
        BGP[0] = (value & 0x03);
    } else if (index == rOBP0) {
        OBP0[2] = (value & 0xc0) >> 6;
        OBP0[1] = (value & 0x30) >> 4;
        OBP0[0] = (value & 0x0c) >> 2;
    } else if (index == rOBP1) {
        OBP1[2] = (value & 0xc0) >> 6;
        OBP1[1] = (value & 0x30) >> 4;
        OBP1[0] = (value & 0x0c) >> 2;
    } else if (index == rWY) WY = value;
    else if (index == rWX) WX = value;
}


void LCDTick(int cycle_length) {
    if (!LCDEnable) return;

    int prevPPUMode = PPUMode;
    dot += cycle_length;
    if (dot >= 456) {
        dot %= 456;
        LY = (LY + 1) % 154;
    }

    if (LY >= 144) PPUMode = M_VBLANK;
    else if (dot >= 252) PPUMode = M_HBLANK;
    else if  (dot >= 80) PPUMode = M_DRAW;
    else PPUMode = M_OAM;

    if (PPUMode != prevPPUMode) { // If change in Mode over this cycle
        if (PPUMode == 2 && M2Int) request_interrupt(LCDSTAT); // OAM Scan
        else if (PPUMode == 0 && M0Int) request_interrupt(LCDSTAT); // Horizontal Blank
        else if (PPUMode == 1) {
            if (M1Int) request_interrupt(LCDSTAT); // VBlank
            request_interrupt(VBLANK);  // Actual VBlank Interrupt
        }
    }

    if (LY == LYC && LYCInt) request_interrupt(LCDSTAT);         
}
