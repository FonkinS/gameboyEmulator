#include <stdio.h>

#include "core/gameboy.h"
#include "app/menu.h"

#include "render.h"



// TODO Other MBCs
// TODO Proper Mem timing (Read/Write happens within instruction, not at the end of it)
int main(int argc, char** argv) {
	if (argc <= 1) {
		printf("File Needed!\n");
		return -1;
	}
    renderInit("Gameboy Emulator");
	GameboyInit("assets/dmg_boot.bin",argv[1]);
    //menuInit("assets/font.bin");
    while (1) {
        GameboyProcessFrame();
        if (!renderFrame()) {break;}
    }
    
    GameboyKill();
    //menuKill();
    renderKill();

    return 0;
}
