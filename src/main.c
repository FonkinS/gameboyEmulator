#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include "gameboy.h"


// TODO Other MBCs
// TODO Fix bad timing (Tetris?)
// TODO Fix Dr. Mario freezing
// TODO Make waits frame based not cpu instruction based
//     speeds up
int main(int argc, char** argv) {
    if (argc <= 1) {
        printf("File Needed!\n");
        return -1;
    }
    if (!GameboyCartridgeLoad(argv[1])) return -1;
    
    GameboyInit();
    
    pthread_t cpu_thread;
    pthread_create(&cpu_thread, NULL, GameboyThreadLoop, NULL);
    GraphicsThreadLoop();


    kill_ppu();
    pthread_cancel(cpu_thread);

    return 0;
}
