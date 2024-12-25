#ifndef FONT_H
#define FONT_H

#ifdef __MACH__
#include "../Desktop/renderDesktop.h"
#else
#include "../Pi/renderPi.h"
#endif

int renderChar(int x, int y, char c, int color);

void renderLine(int x, int y, const char* line, int color);

void renderMultiline(int x, int y, int width, int height, const char* line, int color, int breaktext);



void fontInit(const char* fontFile);

void fontKill();

#endif
