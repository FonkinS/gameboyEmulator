#include "interrupts.h"

void request_interrupt(int type) {
    io_write(rIF, io_read(rIF) | type);
}


void check_interrupts() {
    for (int i = VBLANK; i <= JOYPAD; i <<= 1) { // loops through all types
        if (!(io_read(rIF) & i && IE & i)) continue;
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


void interrupt() {
    if (IME) {
        // Disable IME and IF Flag
        IME = false;
        io_write(rIF, io_read(rIF) & ~interrupt_called);
        
        // PUSH PC to stack
        post(SP-1, (uint8_t) (PC>>8));
        post(SP-2, (uint8_t) PC);
        SP-=2;

        // Call register
        PC = interruptTypeToLocation(interrupt_called);
    }
    interrupt_called = 0;
}
