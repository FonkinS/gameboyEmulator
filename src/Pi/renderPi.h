#ifndef RENDER_PI_H
#define RENDER_PI_H

#include "inputPi.h"


uint8_t screen[160*144];

int renderInit(char* title);
void renderKill();
int renderFrame();


#endif
