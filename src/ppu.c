#include "ppu.h"
//#include "../include/sgf.h"
#include "joypad.h"
#include "lcd.h"
#include <stdint.h>
#include <stdio.h>

#include <linux/fb.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <sys/mman.h>


uint8_t screen[160*144];
uint8_t faux_bg_screen[160*144];

//uint8_t colors[4][3] = {{202, 221, 149}, {139, 162, 106}, {66, 96, 61}, {12, 24, 17}};
uint8_t colors[4][3] = {{186, 218, 85}, {130, 153, 59}, {74,87,34}, {19,22,8}};

int fbfd;
int fb_data_size;
int fb_width;
int fb_height;
int fb_bytes;

uint32_t *fbdata;
uint32_t *row_cache;

int PPUInit(char* title) {
	fbfd = open("/dev/fb0", O_RDWR);
	if (fbfd < 0) printf("FB OPEN ERROR!");
	struct fb_var_screeninfo vinfo;

	ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo);

	fb_width = vinfo.xres;
	fb_height = vinfo.yres;
	fb_bytes = vinfo.bits_per_pixel / 8;

	fb_data_size = fb_width * fb_height * fb_bytes;

	fbdata = mmap(0, fb_data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, (off_t)0);

	row_cache = (uint32_t*) malloc(160*sizeof(uint32_t));

    return 1;
}

int renderFrame() {
	uint32_t *pixel = fbdata + fb_width*24;
	for (int y = 0; y < 144; y++) {
		int line = y*160;
		for (int x = 0; x < 160; x++) {
			uint32_t col = (colors[screen[line+x]][2]) + (colors[screen[line+x]][1] << 8) + (colors[screen[line+x]][0] << 16);
			row_cache[x] = col;
			*(pixel++) = col;
			*(pixel++) = col;
			*(pixel++) = col;
			*(pixel++) = col;
		}
		for (int x = 0; x < 160; x++) {
			uint32_t col = row_cache[x];
			*(pixel++) = col; 
			*(pixel++) = col;
			*(pixel++) = col;
			*(pixel++) = col;
		}
		for (int x = 0; x < 160; x++) {
			uint32_t col = row_cache[x];
			*(pixel++) = col;
			*(pixel++) = col;
			*(pixel++) = col;
			*(pixel++) = col;
		}
	}

    return true;
}



void PPUKill() {
	free(row_cache);
	munmap(fbdata, fb_data_size);
	close(fbfd);
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



