#ifndef PPU_H
#define PPU_H
#include <stdint.h>
#include "cpu.h"
#include "bus.h"
#include "joypad.h"
#include "lcd.h"

#ifdef __MACH__
#include "../Desktop/renderDesktop.h"
#else
#include "../Pi/renderPi.h"
#endif

uint8_t faux_bg_screen[160*144];

void drawScanline(int scanline, int vsync);




#endif
