#ifndef RENDER_H
#define RENDER_H
#include <stdint.h>

#include "input.h"

uint8_t screen[160*144];

int renderInit(char* title);
void renderKill();
int renderFrame();

#endif
