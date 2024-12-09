#include <stdio.h>
#include <time.h>
#include "gameboy.h"


// TODO Other MBCs
// TODO Proper Mem timing (Read/Write happens within instruction, not at the end of it)
//
//
// TODO (IN Raspberry pi)
// - Make drawScanline() like 10x faster
// - Multithread renderFrame?
int main(int argc, char** argv) {
#ifdef __MACH__
	if (argc <= 1) {
		printf("File Needed!\n");
		return -1;
	}
	if (GameboyInit("demoFiles/dmg_boot.bin",argv[1])) {
		return -1;
	}
#else
    if (GameboyInit("/home/pi/gameboyEmulator/demoFiles/dmg_boot.bin","/home/pi/gameboyEmulator/demoFiles/Tetris.gb")) {
        return -1;
    }
#endif
    clock_t begin = clock();
    while (GameboyProcessFrame()) {}
    printf("Time: %f\n", (double)(clock() - begin) / CLOCKS_PER_SEC);
    GameboyKill();

    return 0;
}
