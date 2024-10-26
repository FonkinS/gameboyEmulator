#include "ppu.h"
#include "../include/sgf.h"
#include <stdio.h>

SGFwindow window;

GLubyte background_texture[160*144*3];
SGFsprite bg;

uint8_t colors[4][4] = {{155, 188, 15}, {139, 172, 15}, {48, 98, 48}, {15, 56, 15}};


int init_ppu(char* title) {
    window = SGFCreateWindow(320,288, title, 0);
    bg = SGFCreateBitmapSprite(window, 0, 0, 320, 288, 160, 144, background_texture, 0);
    return 1;
}


void kill_ppu() {
    SGFTerminate();
}


void draw_scanline(int scanline) {
    int bg_tm_start = (io_read(rLCDC) & 8) ? 0x9c00 : 0x9800;
    int bg_td_start = (io_read(rLCDC) & 16) ? 0x8000 : 0x9000;
    int scrollx = io_read(rSCX);
    int scrolly = io_read(rSCY);
    //printf("%i, %i\n", scrollx, scrolly);
    // Note for future self, single row and column of edge tiles might not work
    // Fix scrolling dumbass
    uint8_t bgp = io_read(rBGP);
    int palette[4] = {bgp & 3, (bgp & 12) >> 2, (bgp & 48) >> 4, (bgp & 192) >> 6};

    uint8_t ty = (scanline + scrolly) % 256;
    for (int x = 0; x < 160;x++) {
        uint8_t tx = (x + scrollx) % 256;
        uint8_t tilex = tx / 8;
        uint8_t tiley = ty / 8;

        int tm = read(bg_tm_start + tilex + tiley*32);
        if (bg_td_start == 0x9000) tm = -((int)((int)tm ^ (int)255) + 1); 
        if (tm == -256) tm = 0;

        uint8_t first = read(bg_td_start + (tm * 16) + (ty%8)*2);
        uint8_t second = read(bg_td_start + (tm * 16) + (ty%8)*2+1);

        uint8_t color = (((first >> (7-(tx%8))) & 1) << 1) + ((second >> (7-(tx%8))) & 1);
        background_texture[(scanline * 160 + x)*3] = colors[palette[color]][0];
        background_texture[(scanline * 160 + x)*3+1] = colors[palette[color]][1];
        background_texture[(scanline * 160 + x)*3+2] = colors[palette[color]][2];
    }
}


int render_frame() {
    SGFFillColor(1.0f,0.0f,0.0f,1.0f);

    SGFSetBitmapTexture(bg, 160, 144, background_texture, 0);
    SGFDrawSprite(bg);

    SGFLoopEnd(window);
    return SGFRunning(window);
}
