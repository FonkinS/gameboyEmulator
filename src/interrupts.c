#include "interrupts.h"


uint8_t IF;
uint8_t IE;

bool IME;

int interrupt_called;



void request_interrupt(int type) {
    IF |= type;
}


void check_interrupts() {
    for (int i = VBLANK; i <= JOYPAD; i <<= 1) { // loops through all types
        if (!(IF & i && IE & i)) continue;
        interrupt_called = i;
        break;
    }
}


uint8_t interruptTypeToLocation(uint8_t type) {
    switch (type) {
        case 1:
            return 0x40;
        case 2:
            return 0x48;
        case 4:
            return 0x50;
        case 8:
            return 0x58;
        case 16:
            return 0x60;
        default:
            return 0;
    }
}





uint8_t IFRead() {
    return 0xe0 | IF;
}


void IFWrite(uint8_t value) {
    IF = value;
}

