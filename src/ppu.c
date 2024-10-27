#include "ppu.h"
#include "../include/sgf.h"
#include "joypad.h"
#include <stdio.h>

SGFwindow window;

uint8_t texture[160*144*3];
uint8_t faux_bg_texture[160*144];
SGFsprite bg;

uint8_t colors[4][4] = {{202, 221, 149}, {139, 162, 106}, {66, 96, 61}, {12, 24, 17}};


void key_callback(SGFwindow window, int key, int scancode, int action, int mods);

int init_ppu(char* title) {
    window = SGFCreateWindow(320,288, title, 0);
    bg = SGFCreateBitmapSprite(window, 0, 0, 320, 288, 160, 144, texture, 0);
    SGFSetKeyCallback(window, key_callback);
    return 1;
}


void kill_ppu() {
    SGFTerminate();
}

// TODO FIX BG PALETTE 2 3 MIXED UP
void draw_scanline(int scanline) {
    int bg_tm_start = (io_read(rLCDC) & 8) ? 0x9c00 : 0x9800;
    int bg_td_start = (io_read(rLCDC) & 16) ? 0x8000 : 0x9000;
    int scrollx = io_read(rSCX);
    int scrolly = io_read(rSCY);
    // TODO Fix scrolling edges
    uint8_t bgp = io_read(rBGP);
    int bg_palette[4] = {bgp & 3, (bgp & 12) >> 2, (bgp & 48) >> 4, (bgp & 192) >> 6};

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

        uint8_t color = (((first >> (7-(tx%8))) & 1)) + (((second >> (7-(tx%8))) & 1) << 1);
        texture[(scanline * 160 + x)*3] = colors[bg_palette[color]][0];
        texture[(scanline * 160 + x)*3+1] = colors[bg_palette[color]][1];
        texture[(scanline * 160 + x)*3+2] = colors[bg_palette[color]][2];
        faux_bg_texture[scanline*160+x] = color;

    }

    // TODO Support for Sprite Flipping
    for (int o = 0xfe00; o < 0xfea0; o+=4) {
        uint8_t y = read(o)-16;
        uint8_t x = read(o+1);
        if (scanline-y < 8 && scanline >= y && x != 0 && x < 168) {
            uint8_t tile = read(o+2);
            uint8_t flags = read(o+3);
            uint8_t act_y = scanline-y;

            uint8_t first = read(0x8000 + tile*16 + act_y * 2);
            uint8_t second = read(0x8000 + tile*16 + act_y * 2 + 1);
            
            uint8_t obp = read((flags & 0x10) ? rOBP1 : rOBP0);
            int o_palette[4] = {obp & 3, (obp & 12) >> 2, (obp & 48) >> 4, (obp & 192) >> 6};
            for (int ox = 0; ox < 8; ox++) {
                if (ox+x < 0) continue;
                if (ox+x >= 168) break;
                if (flags & 0x80 && faux_bg_texture[scanline*160+ox+x-8] > 0) continue; // BG Priority
                uint8_t color = (((first >> (7-(ox%8))) & 1)) + (((second >> (7-(ox%8))) & 1) << 1);
                if (color == 0) continue;
                texture[(scanline * 160 + ox+x-8) * 3] = colors[o_palette[color]][0];
                texture[(scanline * 160 + ox+x-8) * 3+1] = colors[o_palette[color]][1];
                texture[(scanline * 160 + ox+x-8) * 3+2] = colors[o_palette[color]][2];

            }
        }
    }
}


int render_frame() {
    SGFFillColor(1.0f,0.0f,0.0f,1.0f);

    SGFSetBitmapTexture(bg, 160, 144, texture, 0);
    SGFDrawSprite(bg);

    SGFLoopEnd(window);
    return SGFRunning(window);
}


void key_callback(SGFwindow window, int key, int scancode, int action, int mods) {
    if      (key == GLFW_KEY_UP) dpad_up = action == GLFW_RELEASE ? false : true;
    else if (key == GLFW_KEY_LEFT) dpad_left = action == GLFW_RELEASE ? false : true;
    else if (key == GLFW_KEY_DOWN) dpad_down = action == GLFW_RELEASE ? false : true;
    else if (key == GLFW_KEY_RIGHT) dpad_right = action == GLFW_RELEASE ? false : true;
    else if (key == GLFW_KEY_BACKSPACE) button_select = action == GLFW_RELEASE ? false : true;
    else if (key == GLFW_KEY_ENTER) button_start = action == GLFW_RELEASE ? false : true;
    else if (key == GLFW_KEY_Z) button_b = action == GLFW_RELEASE ? false : true;
    else if (key == GLFW_KEY_X) button_a = action == GLFW_RELEASE ? false : true;
}
