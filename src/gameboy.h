#ifndef GAMEBOY_H
#define GAMEBOY_H
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include "cpu.h"
#include "ppu.h"
#include "lcd.h"
#include "interrupts.h"
#include "ppu.h"
#include "bus.h"
#include "timer.h"

#define FRAME_DURATION 0.0167427062988f


void GameboyInit();
int GameboyCartridgeLoad(char *p);

int GameboyProcessInstruction();
bool GameboyProcessFrame();



#endif


