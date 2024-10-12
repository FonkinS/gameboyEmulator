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


void draw_bg() {
    for (int i = 0x8000; i < 0x8200;i++) {
        //printf("%X - %X\n", i, data[i]);
    }
    //printf("\n\n\n\n");
    int bg_tm_start = (data[0xff40] & 8) ? 0x9c00 : 0x9800;
    int bg_td_start = (data[0xff40] & 16) ? 0x8000 : 0x9000;
    int scrollx = data[0xff43];
    int scrolly = data[0xff42];
    //printf("%i, %i\n", scrollx, scrolly);
    int tilescrollx = scrollx / 8;
    int tilescrolly = scrolly / 8;
    // Note for future self, single row and column of edge tiles might not work
    // Fix scrolling dumbass
    for (int y = 0; y < 32; y++) {
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
                        background_texture[index] = colors[color][0];
                        background_texture[index+1] = colors[color][1];
                        background_texture[index+2] = colors[color][2];
                        //background_texture[index] = 255;
                        //background_texture[index+1] = 255;
                        //background_texture[index+2] = 255;
                    }
                }
            }
        }
    }
}


int update_frame() {
    SGFFillColor(1.0f,0.0f,0.0f,1.0f);

    draw_bg();
    SGFSetBitmapTexture(bg, 160, 144, background_texture, 0);
    SGFDrawSprite(bg);

    SGFLoopEnd(window);
    return SGFRunning(window);
}
