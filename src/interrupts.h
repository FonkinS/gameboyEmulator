#ifndef INTERRUPTS_H
#define INTERRUPTS_H
#include "cpu.h"

enum INTERRUPT_TYPES {VBLANK=1, LCDSTAT=2, TIMER=4, SERIAL=8, JOYPAD=16};

// bool IME; in CPU.H (to stop recursive includes AHHH)

int interrupt_called;

void interrupt();
uint8_t interruptTypeToLocation(uint8_t type);


void request_interrupt(int type);
void check_interrupts();



#endif
