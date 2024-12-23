#include "renderDesktop.h"
#include <stdio.h>
#include <stdlib.h>



#define WIDTH 160
#define HEIGHT 144

unsigned char *font;
FILE* fontfile;

void renderChar(int x, int y, char c, int color) {
    int loc = 0;
    if (c > 0x40 && c < 0x5B) {
        loc = c - 0x40;
    } else if (c > 0x60 && c < 0x7B) {
        loc = c - 0x60;
    } else if (c >= 0x30 && c < 0x3A) {
        loc = c - 0x30 + 26;
    }

    for (int ty = 0; ty < 8; ty++) {
        for (int tx = 0; tx < 8; tx++) {
            screen[(y+ty) * WIDTH + x+tx] = ((font[loc*8+ty] >> (8-tx)) % 2) * color;
        }
    }
}

void renderLine(int x, int y, const char* line, int color) {
    int i = 0;
    while (line[i] != '\0') {
        if (x+i*8+7 >= WIDTH) {break;}
        renderChar(x+i*8, y, line[i], color);
        i++;
    }
}


void renderMultiline(int x, int y, int width, int height, const char* line, int color, int breaktext) {
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

void fontInit() {
    fontfile = fopen("assets/font.bin", "rb");

    font = (unsigned char*) malloc(36*8 * sizeof(unsigned char));

    int i;
    for (i = 0; i < 36*8; i++) {font[i] = fgetc(fontfile); if (feof(fontfile)) {break;}}

}

void fontKill() {
    fclose(fontfile);
    free(font);
}
