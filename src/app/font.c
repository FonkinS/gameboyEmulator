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

#include "font.h"
#include <stdio.h>
#include <stdlib.h>

#include "../render.h"



#define WIDTH 160
#define HEIGHT 144

unsigned char *font;
FILE* fontfile;

int renderChar(int x, int y, char c, int color) {
    int loc = 0;
    if (c > 0x40 && c < 0x5B) {
        loc = c - 0x40;
    } else if (c > 0x60 && c < 0x7B) {
        loc = c - 0x60;
    } else if (c >= 0x30 && c < 0x3A) {
        loc = c - 0x30 + 27;
    } else if (c == 0x20) {
        loc = 0;
    } else {
        return 0;
    }

    for (int ty = 0; ty < 8; ty++) {
        for (int tx = 0; tx < 8; tx++) {
            screen[(y+ty) * WIDTH + x+tx] = ((font[loc*8+ty] >> (8-tx)) % 2) * color;
        }
    }
    return 1;
}

void renderLine(int x, int y, char* line, int color) {
    int i = 0;
    while (line[i] != '\0') {
        if (x+7 >= WIDTH) {break;}
        if (renderChar(x, y, line[i], color)) x += 8;
        i++;
    }
}


void renderMultiline(int x, int y, int width, int height, char* line, int color, int breaktext) {
    int startx = x;
    int c = 0;
    while (line[c] != '\0') {
        if (line[c] == ' ' && !breaktext) {
            int s;
            for (s = c+1;!(line[s] == ' ') && !(line[s] == '\0'); s++) {}
            if (x+(s-c)*8 >= width) {x = startx; c++; y+=8; if (y >= height) {break;}}
        }
        if (line[c] == '\n') {x = startx; y += 8;c++; if (y >= height) {break;}}
        if (x >= width) {x = startx; y += 8; if (y >= height) {break;}}
        renderChar(x, y, line[c], color);
        c++;
        x += 8;
    }
}

void fontInit(char* fontFile) {
    fontfile = fopen(fontFile, "rb");

    font = (unsigned char*) malloc(36*8 * sizeof(unsigned char));

    int i;
    for (i = 0; i < 36*8; i++) {font[i] = fgetc(fontfile); if (feof(fontfile)) {break;}}

}

void fontKill() {
    fclose(fontfile);
    free(font);
}
