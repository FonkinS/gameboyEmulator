#include "mbc.h"
#include "mbc/mbc0.h"
#include "mbc/mbc1.h"

int mbc;

int MBCInit(uint8_t *data, long long length) {
    int type = data[0x147];
    if (type == 0) {
        mbc = 0;
        MBC0Init(data, length);
        MBCNRead = &MBC0Read;
        MBCNWrite = &MBC0Write;
        MBCNKill = &MBC0Kill;
    } else if (type == 1 || type == 2 || type == 3) {
        mbc = 1;
        MBC1Init(data, length);
        MBCNRead = &MBC1Read;
        MBCNWrite = &MBC1Write;
        MBCNKill = &MBC1Kill;
    } else if (type >= 0xf && type <= 0x13) {
        mbc = 3;
        MBC3Init(data, length);
        MBCNRead = &MBC3Read;
        MBCNWrite = &MBC3Write;
        MBCNKill = &MBC3Kill;
    } else {
        printf("Invalid Cartridge Type: %X!\n", data[0x147]);
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
