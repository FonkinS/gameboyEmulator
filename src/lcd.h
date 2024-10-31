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

int dot;

// TODO Rewrite PPU to reference these
// TODO Actually fix the Dr Mario bug with Setting variables to nill when ppu turned off
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




void LCDTick(int cycle_length);
uint8_t LCDRead(uint16_t index);
void LCDWrite(uint16_t index, uint8_t value);



#endif
