#ifndef PPU_H
#define PPU_H
#include <stdint.h>

uint8_t faux_bg_screen[160*144];

void drawScanline(int scanline);



#endif
