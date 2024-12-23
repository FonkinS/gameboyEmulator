#include "renderPi.h"
#include "inputDesktop.h"

#include <linux/fb.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <sys/mman.h>
#include <stdint.h>


//uint8_t colors[4][3] = {{202, 221, 149}, {139, 162, 106}, {66, 96, 61}, {12, 24, 17}};
uint8_t colors[4][3] = {{186, 218, 85}, {130, 153, 59}, {74,87,34}, {19,22,8}};

int fbfd;
int fb_data_size;
int fb_width;
int fb_height;
int fb_bytes;

uint32_t *fbdata;
uint32_t *row_cache;

int renderInit(char* title) {
	fbfd = open("/dev/fb0", O_RDWR);
	if (fbfd < 0) printf("FB OPEN ERROR!");
	struct fb_var_screeninfo vinfo;

	ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo);

	fb_width = vinfo.xres;
	fb_height = vinfo.yres;
	fb_bytes = vinfo.bits_per_pixel / 8;

	fb_data_size = fb_width * fb_height * fb_bytes;

	fbdata = mmap(0, fb_data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, (off_t)0);

	memset(fbdata,0, fb_data_size);

	row_cache = (uint32_t*) malloc(160*sizeof(uint32_t));

    inputInit();

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

    inputTick(NULL);

    return true;
}



void renderKill() {
	free(row_cache);
	munmap(fbdata, fb_data_size);
	close(fbfd);
}


