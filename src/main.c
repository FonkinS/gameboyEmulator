#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include "gameboy.h"


// TODO Other MBs
// TODO Fix bad timing (Tetris?)
// TODO Fix Dr. Mario menu
int main() {
    if (!GameboyCartridgeLoad("demoFiles/Dr. Mario.gb")) return -1;
    
    GameboyInit();
    
    pthread_t cpu_thread;
    pthread_create(&cpu_thread, NULL, GameboyThreadLoop, NULL);
    GraphicsThreadLoop();


    kill_ppu();
    pthread_cancel(cpu_thread);

    return 0;
}
