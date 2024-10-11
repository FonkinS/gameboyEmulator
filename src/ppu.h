#ifndef PPU_H
#define PPU_H
#include <stdint.h>
#include "cpu.h"


int init_ppu();
void kill_ppu();
int update_frame();
void draw_bg();




#endif
