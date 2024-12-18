#ifndef RENDER_DESKTOP_H
#define RENDER_DESKTOP_H

#include "ppu.h"

int renderInit(char* title);
void renderKill();
int renderFrame();

#endif
