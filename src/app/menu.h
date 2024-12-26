#ifndef MENU_H
#define MENU_H

#include "../input.h"

void menuInit(const char* fontFile, void (*callback)());
void menuKill();
void menuTick();

void placeCursor(int index);
void menuInputCallback(enum BUTTON button, enum PRESS_TYPE type);

#endif
