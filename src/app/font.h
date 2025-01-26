/*
 * Copyright (C) 2025 Aaron Kainu
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef FONT_H
#define FONT_H

#include "../render.h"
#include <stdio.h>

int renderChar(int x, int y, char c, int color);

void renderLine(int x, int y, char* line, int color);

void renderMultiline(int x, int y, int width, int height, char* line, int color, int breaktext);



void fontInit(char* fontFile);

void fontKill();

#endif
