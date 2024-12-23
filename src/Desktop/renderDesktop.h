#ifndef RENDER_DESKTOP_H
#define RENDER_DESKTOP_H

#include "inputDesktop.h"

uint8_t screen[160*144];

int renderInit(char* title);
void renderKill();
int renderFrame();

#endif
