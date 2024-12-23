#include <stdio.h>

#include "core/gameboy.h"
#ifdef __MACH__
#include "Desktop/renderDesktop.h"
#else
#include "Pi/renderPi.h"
#endif



// TODO Other MBCs
// TODO Proper Mem timing (Read/Write happens within instruction, not at the end of it)
int main(int argc, char** argv) {
	if (argc <= 1) {
		printf("File Needed!\n");
		return -1;
	}
	if (GameboyInit("assets/dmg_boot.bin",argv[1])) {
		return -1;
	}
    renderInit("GameboyEmulator");
    while (1) {
        GameboyProcessFrame();
        if (!renderFrame()) {break;}
    }
    
    GameboyKill();
    renderKill();

    return 0;
}
