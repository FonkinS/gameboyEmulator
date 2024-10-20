#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include "cpu.h"
#include "ppu.h"
#include "lcd.h"
#include "gameboy.h"

int main() {
    GameboyInit();

    if (!GameboyCartridgeLoad()) return -1;
    
    
    pthread_t cpu_thread;
    pthread_create(&cpu_thread, NULL, GameboyThreadLoop, NULL);
    GraphicsThreadLoop();


    kill_ppu();
    pthread_cancel(cpu_thread);

    return 0;
}
