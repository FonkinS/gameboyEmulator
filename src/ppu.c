#include "ppu.h"
#include "../include/sgf.h"
#include "joypad.h"
#include "lcd.h"
#include <stdint.h>
#include <stdio.h>

SGFwindow window;

uint8_t texture[160*144*3];
uint8_t faux_bg_texture[160*144];
SGFsprite bg;

uint8_t colors[4][4] = {{202, 221, 149}, {139, 162, 106}, {66, 96, 61}, {12, 24, 17}};


void key_callback(SGFwindow window, int key, int scancode, int action, int mods);

int PPUInit(char* title) {
    window = SGFCreateWindow(640,576, title, 0);
    bg = SGFCreateBitmapSprite(window, 0, 0, 640, 576, 160, 144, texture, 0);
    SGFSetKeyCallback(window, key_callback);
    return 1;
}


void PPUKill() {
    SGFTerminate();
}

void drawScanline(int scanline) {
    if (BGWinEnable) {
        // TODO Fix scrolling edges
        uint8_t ty = (scanline + SCY) % 256;
        for (int x = 0; x < 160;x++) {
            uint8_t tx = (x + SCX) % 256;
            uint8_t tilex = tx / 8;
            uint8_t tiley = ty / 8;

            uint8_t index = BusRead(BGTileMap + tilex + tiley*32);
            int tile = BGWinTileData;
            if (BGWinTileData == 0x8000) tile += (int)((uint8_t)index) * 16;
            if (BGWinTileData == 0x9000) tile += (int)((int8_t)index) * 16;
            // make tmu
            uint8_t first = BusRead(tile + (ty%8)*2);
            uint8_t second = BusRead(tile + (ty%8)*2+1);

            uint8_t color = (((first >> (7-(tx%8))) & 1)) + (((second >> (7-(tx%8))) & 1) << 1);
            texture[(scanline * 160 + x)*3] = colors[BGP[color]][0];
            texture[(scanline * 160 + x)*3+1] = colors[BGP[color]][1];
            texture[(scanline * 160 + x)*3+2] = colors[BGP[color]][2];
            faux_bg_texture[scanline*160+x] = color;

        }


        if (WinEnable && WX >= 0 && WX <= 166 && WY >= 0 && WY <= 143) {
            uint8_t ty = (scanline + WY);
            for (int x = 0; x < 160;x++) {
                uint8_t tx = (x + WX-7);
                if (tx < 0 || tx >= 160) continue;
                uint8_t tilex = tx / 8;
                uint8_t tiley = ty / 8;

                uint8_t index = BusRead(WinTileMap + tilex + tiley*32);
                int tile = BGWinTileData;
                if (BGWinTileData == 0x8000) tile += (int)((uint8_t)index) * 16;
                if (BGWinTileData == 0x9000) tile += (int)((int8_t)index) * 16;
                //printf("%.4X: %.4X (%.2X)\n", BGWinTileData, tile, index);

                uint8_t first = BusRead(tile + (ty%8)*2);
                uint8_t second = BusRead(tile + (ty%8)*2+1);

                uint8_t color = (((first >> (7-(tx%8))) & 1)) + (((second >> (7-(tx%8))) & 1) << 1);
                texture[(scanline * 160 + x)*3] = colors[BGP[color]][0];
                texture[(scanline * 160 + x)*3+1] = colors[BGP[color]][1];
                texture[(scanline * 160 + x)*3+2] = colors[BGP[color]][2];
                faux_bg_texture[scanline*160+x] = color;

            }
        }
    } else {
        /*for (int x = 0; x < 160; x++) {
            texture[(scanline * 160 + x)*3] = colors[0][0];
            texture[(scanline * 160 + x)*3+1] = colors[0][1];
            texture[(scanline * 160 + x)*3+2] = colors[0][2];
        }*/
    }
    

    // TODO Support for Sprite Flipping
    // TODO Support For 8x16 sprites
    // TODO Pokemon Blue long blank screen pauses?
    // Objects
    if (OBJEnable) {
        for (int o = 0xfe00; o < 0xfea0; o+=4) {
            uint8_t y = BusRead(o)-16;
            uint8_t x = BusRead(o+1);
            if (scanline-y < 8 && scanline >= y && x != 0 && x < 168) {
                uint8_t tile = BusRead(o+2);
                uint8_t flags = BusRead(o+3);
                uint8_t act_y = scanline-y;

                uint8_t first = BusRead(0x8000 + tile*16 + act_y * 2);
                uint8_t second = BusRead(0x8000 + tile*16 + act_y * 2 + 1);
                
                uint8_t *obp = (flags & 0x10) ? OBP1 : OBP0;
                for (int ox = 0; ox < 8; ox++) {
                    if (ox+x < 0) continue;
                    if (ox+x >= 168) break;
                    if (flags & 0x80 && faux_bg_texture[scanline*160+ox+x-8] > 0) continue; // BG Priority
                    uint8_t color;
                    if (flags & 0x20) color = (((first >> ((ox%8))) & 1)) + (((second >> ((ox%8))) & 1) << 1); // X Flip
                    else color = (((first >> (7-(ox%8))) & 1)) + (((second >> (7-(ox%8))) & 1) << 1);
                    if (color == 0) continue;
                    texture[(scanline * 160 + ox+x-8) * 3] = colors[obp[color-1]][0];
                    texture[(scanline * 160 + ox+x-8) * 3+1] = colors[obp[color-1]][1];
                    texture[(scanline * 160 + ox+x-8) * 3+2] = colors[obp[color-1]][2];

                    }
                }
            }
    }
}

int renderFrame() {
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
