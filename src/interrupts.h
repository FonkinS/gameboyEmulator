#ifndef INTERRUPTS_H
#define INTERRUPTS_H
#include "cpu.h"

void request_interrupt(int type);
void check_interrupts(int* interrupt_called);



#endif
