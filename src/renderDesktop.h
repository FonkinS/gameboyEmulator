#ifndef RENDER_DESKTOP_H
#define RENDER_DESKTOP_H

#include "ppu.h"
#include "inputDesktop.h"

int renderInit(char* title);
void renderKill();
int renderFrame();

#endif
