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


void GameboyInit();
int GameboyCartridgeLoad(char *p);
void *GameboyThreadLoop();
void GraphicsThreadLoop();



#endif


/*set_af(0x01b0);
    set_bc(0x0013);
    set_de(0x00d8);
    set_hl(0x014d);
    SP =  0xFFFE;
    data[0xff04] = 0xbd; // SET BACK TO ZERO WHEN INIT
    data[0xFF05] = 0x00;
    data[0xFF06] = 0x00;
    data[0xFF07] = 0x00;
    data[0xFF10] = 0x80;
    data[0xFF11] = 0xBF;
    data[0xFF12] = 0x3F;
    data[0xFF14] = 0xBF;
    data[0xFF16] = 0x3F;
    data[0xFF17] = 0x00;
    data[0xFF19] = 0xBF;
    data[0xFF1A] = 0x7F;
   data[0xFF1B] = 0xFF;
    data[0xFF1C] = 0x9F;
    data[0xFF1E] = 0xBF;
    data[0xFF20] = 0xFF;
    data[0xFF21] = 0x00;
    data[0xFF22] = 0x00;
    data[0xFF23] = 0xBF;
    data[0xFF24] = 0x77;
    data[0xFF25] = 0xF3;
    data[0xFF26] = 0xF1;
    data[0xFF40] = 0x91;
    data[0xFF42] = 0x00;
    data[0xFF43] = 0x00;
    data[0xFF45] = 0x00;
    data[0xFF47] = 0xFC;
    data[0xFF48] = 0xFF;
    data[0xFF49] = 0xFF;
    data[0xFF4A] = 0x00;
    data[0xFF4B] = 0x00;
    data[0xFFFF] = 0x00;

    PC = 0x100;
    SP = 0xfffe;
    serial_interrupt = 0;

    a = 0x01;
    f = 0xb0;
    b = 0x00;
    c = 0x13;
    d = 0x00;
    e = 0xd8;
    h = 0x01;
    l = 0x4d;*/

    
    


