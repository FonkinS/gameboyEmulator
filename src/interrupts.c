#include "interrupts.h"

void request_interrupt(int type) {
    data[0xff0f] |= type;
}


void check_interrupts(int *interrupt_called) {
    if (data[0xff0f] & 31 && IME) {
        if (data[0xff0f] & 1 && data[0xffff] & 1) *interrupt_called = VBLANK;
        if (data[0xff0f] & 2 && data[0xffff] & 2) *interrupt_called = LCDSTAT;
        if (data[0xff0f] & 4 && data[0xffff] & 4) *interrupt_called = TIMER;
        if (data[0xff0f] & 8 && data[0xffff] & 8) *interrupt_called = SERIAL;
        if (data[0xff0f] & 16 && data[0xffff] & 16) *interrupt_called = JOYPAD;
    }
}
