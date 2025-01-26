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

#include "../render.h"
#include "../input.h"

#include <linux/fb.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <sys/mman.h>
#include <stdint.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


uint8_t colors[4][3] = {{186, 218, 85}, {130, 153, 59}, {74,87,34}, {19,22,8}};

int fbfd;
int fb_data_size;
int fb_width;
int fb_height;
int fb_bytes;

uint32_t *fbdata;
uint32_t *row_cache;

// The main variable which all drawing functions will reference
uint8_t screen[160*144];


/* The Raspberry Pi rendering utilised the Linux Framebuffer, which is a 
 * device found in /dev/fb0. Each pixel is represented by 4 bytes, or one
 * 32bit number, with them all being neatly packed in memory. (Allowing for 
 * easy pointer arithmetic in the renderFrame function)*/
int renderInit(char* title) {
    // Opens the Linux Framebuffer for access
	fbfd = open("/dev/fb0", O_RDWR);
	if (fbfd < 0) printf("FB OPEN ERROR!");
	struct fb_var_screeninfo vinfo;
	ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo);

    // Retrieves data about the Linux Framebuffer
	fb_width = vinfo.xres;
	fb_height = vinfo.yres;
	fb_bytes = vinfo.bits_per_pixel / 8;
	fb_data_size = fb_width * fb_height * fb_bytes;

    // Maps the data into memory, and clears it
	fbdata = mmap(0, fb_data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, (off_t)0);
	memset(fbdata,0, fb_data_size);

    // Inits a cache which will be used in the core renderloop for faster access
	row_cache = (uint32_t*) malloc(160*sizeof(uint32_t));

    inputInit();

    return 1;
}


/* Significant code-duplication is used in the Pi version of RenderFrame. This is entirely
 * done to squeeze as much speed as possible. Since the output display is 640x480, and the 
 * gameboy uses a 160x144 display, we can upscale that to 640x432 (which is a 4x3 upscaling)
 * 
 * Therefore the gameboy rendering will happen 24 pixels lower than the top of the screen
 * in order to center the window vertically.
 * 
 * Since the rendering is upscaled by a size of 4x3, each row of pixels is rendered 3 times,
 * with each pixel in the row being rendered 4 times.
 *
 * To make this assigning faster, pointer arithmetic is used*/
int renderFrame() {
    // Sets pixel pointer to be start of the screen (24 pixels down from the top of the window)
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

    // The NULL is due to the inputTick being a virtual function in input.h, and the desktop
    // version requiring a pointer to the GLFWwindow. (and the pi version doesn't)
    inputTick(NULL);

    return true;
}



void renderKill() {
	free(row_cache);
	munmap(fbdata, fb_data_size);
	close(fbfd);
}


