#include <stdbool.h>
#include <stdio.h>

#include "core/gameboy.h"
#include "app/menu.h"

#include "input.h"
#include "render.h"

bool gameboy_running = false;

void gameboyEndCallback();

void gameboyStartCallback(const char* gamepath) {
    gameboy_running = true;
    menuKill();
    GameboyInit("assets/dmg_boot.bin", gamepath, gameboyEndCallback);
}

void gameboyEndCallback() {
	gameboy_running = false;
	GameboyKill();
    menuInit(gameboyStartCallback);
}

// TODO Other MBCs
// TODO Proper Mem timing (Read/Write happens within instruction, not at the end of it)
int main() {
    menuInit(gameboyStartCallback);
    renderInit("Gameboy Emulator");
    while (1) {
        if (gameboy_running) GameboyProcessFrame();
        if (!renderFrame()) {break;}
    }
    
    if (gameboy_running) GameboyKill();
    else menuKill();
    renderKill();

    return 0;
}
