#include "mbc.h"
#include "mbc/mbc0.h"
#include "mbc/mbc1.h"
#include "mbc/mbc3.h"

int mbc;

bool battery_enabled = false;


int MBCInit(uint8_t *data, long long length, char* save_filename) {
    int type = data[0x147];
    battery_enabled = false;
    if (type == 0) {
        mbc = 0;
        MBC0Init(data, length);
        MBCNRead = &MBC0Read;
        MBCNWrite = &MBC0Write;
        MBCNKill = &MBC0Kill;
    } else if (type == 1 || type == 2 || type == 3) {
        mbc = 1;
        if (type == 3) {battery_enabled = true; MBC1LoadData(save_filename);}
        MBC1Init(data, length, battery_enabled);
        MBCNRead = &MBC1Read;
        MBCNWrite = &MBC1Write;
        MBCNKill = &MBC1Kill;
        MBCNSaveData = &MBC1SaveData;
    } else if (type >= 0xf && type <= 0x13) {
        mbc = 3;
        if (type == 0xf || type == 0x10 || type == 0x13) {battery_enabled = true; MBC3LoadData(save_filename);}
        MBC3Init(data, length, battery_enabled);
        MBCNRead = &MBC3Read;
        MBCNWrite = &MBC3Write;
        MBCNKill = &MBC3Kill;
        MBCNSaveData = &MBC3SaveData;
    } else {
        // TODO MAKE THIS NOT CRASH, BUT DISPLAY IN MENU
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


void MBCSaveData(char* filename) {
    if (battery_enabled) {
        (*MBCNSaveData)(filename);
    }
}

void MBCKill() {
    (*MBCNKill)();
}
