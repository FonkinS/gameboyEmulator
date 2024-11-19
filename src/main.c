#include <stdio.h>
#include "gameboy.h"


// TODO Other MBCs
// TODO Proper Mem timing (Read/Write happens within instruction, not at the end of it)
//
//
// TODO (IN Raspberry pi)
// - Make drawScanline() like 10x faster
// - Multithread renderFrame?
int main(int argc, char** argv) {
    if (argc <= 1) {
        printf("File Needed!\n");
        return -1;
    }

    if (GameboyInit("demoFiles/dmg_boot.bin",argv[1])) {
        return -1;
    }
    while (GameboyProcessFrame()) {}
    GameboyKill();

    return 0;
}
