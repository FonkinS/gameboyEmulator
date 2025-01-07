#include <stdbool.h>

#include "core/gameboy.h"
#include "app/menu.h"
#include "render.h"

// TODO PROPER INIT OF SCREEN AND AUDIO

/* The following functions are used to start and stop the emulator, 
 * going to and from the menu screen */
bool gameboy_running = false;
void gameboyEndCallback();
void gameboyStartCallback(char* gamepath);

// Called when a game is selected in the menu
void gameboyStartCallback(char* gamepath) {
    gameboy_running = true;
    menuKill();
    GameboyInit("assets/dmg_boot.bin", gamepath, gameboyEndCallback);
}

// Called when the SPECIAL button is pressed, returns to the menu
void gameboyEndCallback() {
	gameboy_running = false;
	GameboyKill();
    menuInit(gameboyStartCallback);
}




/* The Main function, which inits the Render Pipeline, and menu, then
 * starts the main render/game-loop */
int main() {
    // Init
    menuInit(gameboyStartCallback);
    renderInit("Gameboy Emulator");

    // Game-/render- loop
    while (1) {
        if (gameboy_running) GameboyProcessFrame();
        if (!renderFrame()) {break;}
    }
    
    // Destroy when the window is closed (Technically will only happen on Desktop)
    if (gameboy_running) GameboyKill();
    else menuKill();
    renderKill();

    return 0;
}
