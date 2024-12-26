#include <stdbool.h>
#include <stdio.h>

#include "core/gameboy.h"
#include "app/menu.h"

#include "render.h"

bool gameboy_running = false;

void gameboyEndCallback();

void gameboyStartCallback(char* gamepath) {
    gameboy_running = true;
    GameboyInit("assets/dmg_boot.bin", gamepath, gameboyEndCallback);
	menuKill();
}

void gameboyEndCallback() {
	gameboy_running = false;
	GameboyKill();
	menuInit("assets/font.bin", gameboyStartCallback);
}

// TODO Other MBCs
// TODO Proper Mem timing (Read/Write happens within instruction, not at the end of it)
int main() {
    renderInit("Gameboy Emulator");
    menuInit("assets/font.bin", gameboyStartCallback);
    while (1) {
        if (gameboy_running) GameboyProcessFrame();
        else menuTick();
        if (!renderFrame()) {break;}
    }
    
    if (gameboy_running) GameboyKill();
	else menuKill();
    renderKill();

    return 0;
}
