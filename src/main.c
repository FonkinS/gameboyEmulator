#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
#include "gameboy.h"


// TODO Other MBCs
// TODO Fix Dr. Mario freezing
// TODO Make Subfiles be standardised in convention
// TODO Proper Mem timing (Read/Write happens within instruction, not at the end of it)
int main(int argc, char** argv) {
    if (argc <= 1) {
        printf("File Needed!\n");
        return -1;
    }
    if (!GameboyCartridgeLoad(argv[1])) return -1;
    
    GameboyInit();
    
    while (GameboyProcessFrame()) {}

    return 0;
}
