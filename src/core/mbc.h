#ifndef MBC_H
#define MBC_H
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "mbc/mbc0.h"
#include "mbc/mbc1.h"
#include "mbc/mbc3.h"

int MBCInit(uint8_t *data, long long length, char* save_filename);
uint8_t MBCRead(uint16_t index);
void MBCWrite(uint16_t index, uint8_t value);

void MBCKill();

void MBCSaveData(char* filename);

extern int mbc;

// Function pointers for read and write
uint8_t (*MBCNRead)(uint16_t);
void (*MBCNWrite)(uint16_t, uint8_t);
void (*MBCNKill)();

void (*MBCNSaveData)(char*);

#endif
