#ifndef PPU_H
#define PPU_H
#include <stdint.h>
#include "cpu.h"
#include "bus.h"
#include "joypad.h"
#include "lcd.h"


int PPUInit(char* title);
void PPUKill();
int renderFrame();
void drawScanline(int scanline, int vsync);




#endif
