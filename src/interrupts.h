#ifndef INTERRUPTS_H
#define INTERRUPTS_H
#include <stdint.h>
#include <stdbool.h>

enum INTERRUPT_TYPES {VBLANK=1, LCDSTAT=2, TIMER=4, SERIAL=8, JOYPAD=16};

uint8_t IF;
uint8_t IE;

bool IME;

int interrupt_called;

uint8_t interruptTypeToLocation(uint8_t type);


void request_interrupt(int type);
void check_interrupts();

uint8_t IFRead();
void IFWrite(uint8_t value);


#endif
