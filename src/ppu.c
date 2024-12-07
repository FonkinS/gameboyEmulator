#include "ppu.h"
//#include "../include/sgf.h"
#include "joypad.h"
#include "lcd.h"
#include <stdint.h>
#include <stdio.h>

#include <SDL2/SDL.h>



SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;

SDL_Rect texture_dst_rect;


uint8_t screen[160*144];
uint8_t faux_bg_screen[160*144];

//uint8_t colors[4][3] = {{202, 221, 149}, {139, 162, 106}, {66, 96, 61}, {12, 24, 17}};
uint8_t colors[4][3] = {{186, 218, 85}, {130, 153, 59}, {74,87,34}, {19,22,8}};



const char* vertexShader = "#version 330 core\n" \
                           "layout (location = 0) in vec3 aPos;\n" \
                           "layout (location = 1) in vec2 aTexCoord;\n" \
                           "uniform vec2 pos;\n" \
                           "out vec2 texCoord;\n" \
                           "void main(){\n" \
                           "\tgl_Position = vec4(aPos.x + pos.x, aPos.y + pos.y, aPos.z, 1.0);\n" \
                           "\ttexCoord = aTexCoord;\n" \
                           "}\n";


const char* fragmentShader = "#version 330 core\n" \
                            "out vec4 FragColor;\n" \
                            "in vec2 texCoord;\n" \
                            "uniform sampler2D inTexture;\n" \
                            "uniform vec3 colors[4];\n" \
                            "void main() {\n" \
                            "\tint col = int(texture(inTexture, texCoord).r*256);\n" \
                            "\tFragColor = vec4(colors[col],1.0f);\n" \
                            "}\n";




int PPUInit(char* title) {
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "0");
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640,480, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, 160, 144);
    
    texture_dst_rect.x = 0;
    texture_dst_rect.y = 0;
    texture_dst_rect.w = 640;
    texture_dst_rect.h = 480;
    //float colors[4][3] = {{0.79, 0.86, 0.58}, {0.54, 0.63, 0.41}, {0.26, 0.37, 0.24}, {0.05, 0.09, 0.07}};
    return 1;
}

int renderFrame() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    void *pixels;
    int pitch;
    if (SDL_LockTexture(texture, NULL, &pixels, &pitch) == 0) {
        uint8_t *pixel_data = (uint8_t*) pixels;
        for (int y = 0; y < 144; y++) {
            for (int x = 0; x < 160; x++) {
                pixel_data[(y * 160 + x) * 4+2] = colors[screen[y * 160 + x]][0];
                pixel_data[(y * 160 + x) * 4+1] = colors[screen[y * 160 + x]][1];
                pixel_data[(y * 160 + x) * 4+0] = colors[screen[y * 160 + x]][2];
            }
        }
        SDL_UnlockTexture(texture);
    }

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) return false;
        else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            SDL_KeyCode key = event.key.keysym.sym;
            if      (key == SDLK_UP) dpad_up = event.type == SDL_KEYUP ? false : true;
            else if (key == SDLK_LEFT) dpad_left = event.type == SDL_KEYUP ? false : true;
            else if (key == SDLK_DOWN) dpad_down = event.type == SDL_KEYUP ? false : true;
            else if (key == SDLK_RIGHT) dpad_right = event.type == SDL_KEYUP ? false : true;
            else if (key == SDLK_BACKSPACE) button_select = event.type == SDL_KEYUP ? false : true;
            else if (key == SDLK_RETURN) button_start = event.type == SDL_KEYUP ? false : true;
            else if (key == SDLK_z) button_b = event.type == SDL_KEYUP ? false : true;
            else if (key == SDLK_x) button_a = event.type == SDL_KEYUP ? false : true;
            else if (key == SDLK_q) exit(0);

        }
    }

    return true;
}



void PPUKill() {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// TODO Notes FOr TOmorrow
// - Table of which objects visible
// - STRUCTURE for objects
int frame = 0;
void drawScanline(int scanline, int vsync) {
    if (scanline == 0) frame++;
    if ((frame+scanline) % vsync != 0) return;
    if (BGWinEnable) {
        // TODO Fix scrolling edges
        uint8_t y = (scanline + SCY) % 256;
        uint8_t tiley = y / 8;
        uint8_t ty = (scanline + SCY) % 8; 
        for (int tilex = 0; tilex < 32; tilex++) {
            uint8_t index = BusRead(BGTileMap + tilex + tiley*32);
            int tile = BGWinTileData + (int)(BGWinTileData == 0x8000 ? (uint8_t)index : (int8_t)index) * 16;
            uint8_t first = BusRead(tile + ty*2);
            uint8_t second = BusRead(tile + ty*2+1);
            for (int tx = 0; tx < 8; tx++) {
                uint8_t x = (tilex * 8 + tx + SCX) % 256;
                if (x < 0 || x >= 160) continue;
                screen[scanline*160+x] = BGP[((first >> (7-tx)) & 1) + (((second >> (7-tx)) & 1) << 1)];
            }
        }

        if (WinEnable && WX >= 0 && WX <= 166 && WY >= 0 && WY <= 143) {
            uint8_t tiley = y / 8;
            uint8_t ty = (scanline + WY) % 8; 
            for (int tilex = WX / 8; tilex < 21 + WX / 8; tilex++) {
                uint8_t index = BusRead(WinTileMap + tilex + tiley*32);
                int tile = BGWinTileData + (int)(BGWinTileData == 0x8000 ? (uint8_t)index : (int8_t)index) * 16;
                uint8_t first = BusRead(tile + ty*2);
                uint8_t second = BusRead(tile + ty*2+1);
                for (int tx = 0; tx < 8; tx++) {
                    uint8_t wx = tilex * 8 + tx;
                    if (wx < 0 || wx >= 160) continue;
                    screen[scanline*160+wx] = BGP[((first >> (7-tx)) & 1) + (((second >> (7-tx)) & 1) << 1)];
                }
            }
        }
    }     

    // TODO Support for Sprite Flipping
    // TODO Support For 8x16 sprites
    // TODO Pokemon Blue long blank screen pauses?
    // Objects
    if (OBJEnable) {
        for (int o = 0xfe00; o < 0xfea0; o+=4) {
            uint8_t y = BusRead(o)-16;
            uint8_t x = BusRead(o+1);
            if (scanline-y < OBJSize && scanline >= y && x != 0 && x < 168) {
                uint8_t tile = BusRead(o+2);
                uint8_t flags = BusRead(o+3);
                uint8_t act_y = scanline-y;

                uint8_t first = BusRead(0x8000 + tile*16 + act_y * 2);
                uint8_t second = BusRead(0x8000 + tile*16 + act_y * 2 + 1);
                
                uint8_t *obp = (flags & 0x10) ? OBP1 : OBP0;
                for (int ox = 0; ox < 8; ox++) {
                    if (ox+x < 0) continue;
                    if (ox+x >= 168) break;
                    if (flags & 0x80 && faux_bg_screen[scanline*160+ox+x-8] > 0) continue; // BG Priority
                    uint8_t color;
                    if (flags & 0x20) color = (((first >> ((ox%8))) & 1)) + (((second >> ((ox%8))) & 1) << 1); // X Flip
                    else color = (((first >> (7-(ox%8))) & 1)) + (((second >> (7-(ox%8))) & 1) << 1);
                    if (color == 0) continue;
                    screen[scanline * 160 + ox+x-8] = obp[color-1];
                    }
                }
            }
    }
}



