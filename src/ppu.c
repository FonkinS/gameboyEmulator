#include "ppu.h"
#include "../include/sgf.h"
#include <stdio.h>

SGFwindow window;

GLubyte background_texture[160*144*3];
SGFsprite bg;

uint8_t colors[4][4] = {{155, 188, 15}, {139, 172, 15}, {48, 98, 48}, {15, 56, 15}};


int init_ppu() {
    window = SGFCreateWindow(320,288, "Gameboy", 0);
    bg = SGFCreateBitmapSprite(window, 0, 0, 320, 288, 160, 144, background_texture, 0);
    return 1;
}


void kill_ppu() {
    SGFTerminate();
}


void draw_scanline(int scanline) {
    int bg_tm_start = (data[0xff40] & 8) ? 0x9c00 : 0x9800;
    int bg_td_start = (data[0xff40] & 16) ? 0x8000 : 0x9000;
    int scrollx = data[0xff43];
    int scrolly = data[0xff42];
    //printf("%i, %i\n", scrollx, scrolly);
    // Note for future self, single row and column of edge tiles might not work
    // Fix scrolling dumbass
    int palette[4] = {data[0xff47] & 3, (data[0xff47] & 12) >> 2, (data[0xff47] & 48) >> 4, (data[0xff47] & 192) >> 6};

    uint8_t ty = (scanline + scrolly) % 256;
    for (int x = 0; x < 160;x++) {
        uint8_t tx = (x + scrollx) % 256;
        uint8_t tilex = tx / 8;
        uint8_t tiley = ty / 8;

        int tm = data[bg_tm_start + tilex + tiley*32];
        if (bg_td_start == 0x9000) tm = -((int)((int)tm ^ (int)255) + 1); 
        if (tm == -256) tm = 0;

        uint8_t first = data[bg_td_start + (tm * 16) + (ty%8)*2];
        uint8_t second = data[bg_td_start + (tm * 16) + (ty%8)*2+1];

        uint8_t color = (((first >> (7-(tx%8))) & 1) << 1) + ((second >> (7-(tx%8))) & 1);
        background_texture[(scanline * 160 + x)*3] = colors[palette[color]][0];
        background_texture[(scanline * 160 + x)*3+1] = colors[palette[color]][1];
        background_texture[(scanline * 160 + x)*3+2] = colors[palette[color]][2];
    }
    /*for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 32; x++) {
            int bg_x = (32+(x-tilescrollx)) % 32;
            int bg_y = (32+(y-tilescrolly)) % 32;
            if (bg_x < 20 && bg_y < 18) {
                int tm = data[bg_tm_start + x + y*32];
                if (bg_td_start == 0x9000) tm = -((int)((int)tm ^ (int)255) + 1); 
                if (tm == -256) tm = 0;
                for (int ty = 0; ty < 8; ty++) {
                    if (bg_y * 8 + ty-scrolly%8 < 0) continue;
                    uint8_t first = data[bg_td_start + (tm * 16)+ty*2]; 
                    uint8_t second = data[bg_td_start + (tm * 16)+ty*2+1]; 
                    for (int tx = 0; tx < 8; tx++) {
                        if (bg_x * 8 + tx-scrollx%8 < 0) continue;
                        uint8_t color = (((first >> (7-tx)) & 1) << 1) + ((second >> (7-tx)) & 1);
                        //uint16_t index = (bg_y*160*8 + (ty-scrolly%8)*160 + bg_x*8 + (tx-scrollx%8)) * 3;
                        int index = ((bg_y * 8 + (ty-(scrolly%8))) * 160 + bg_x*8+(tx-scrollx%8)) * 3;
                        background_texture[index] = colors[palette[color]][0];
                        background_texture[index+1] = colors[palette[color]][1];
                        background_texture[index+2] = colors[palette[color]][2];
                        //background_texture[index] = 255;
                        //background_texture[index+1] = 255;
                        //background_texture[index+2] = 255;
                    }
                }
            }
        }
    }*/
}


int render_frame() {
    SGFFillColor(1.0f,0.0f,0.0f,1.0f);

    SGFSetBitmapTexture(bg, 160, 144, background_texture, 0);
    SGFDrawSprite(bg);

    SGFLoopEnd(window);
    return SGFRunning(window);
}
