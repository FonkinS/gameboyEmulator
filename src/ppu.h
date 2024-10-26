#ifndef PPU_H
#define PPU_H
#include <stdint.h>
#include "cpu.h"


int init_ppu(char* title);
void kill_ppu();
int render_frame();
void draw_scanline(int scanline);




#endif
