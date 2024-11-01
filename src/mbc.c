#include "mbc.h"
#include "mbc/mbc0.h"

int MBCInit(uint8_t *data, long long length) {
    switch (data[0x147]) {
        case 0x0:
            mbc = 0;
            MBC0Init(data, length);
            MBCNRead = &MBC0Read;
            MBCNWrite = &MBC0Write;
            MBCNKill = &MBC0Kill;
            break;
        default:
            printf("Invalid Cartridge Type!\n");
            return -1;
    }

    return 0;
}


uint8_t MBCRead(uint16_t index) {
    return (*MBCNRead)(index);
}


void MBCWrite(uint16_t index, uint8_t value) {
    (*MBCNWrite)(index, value);
}


void MBCKill() {
    (*MBCNKill)();
}
