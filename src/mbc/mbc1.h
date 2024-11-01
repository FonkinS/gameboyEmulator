#ifndef MBC1_H
#define MBC1_H
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

void MBC1Init(uint8_t *data, long long length);
uint8_t MBC1Read(uint16_t index);
void MBC1Write(uint16_t index, uint8_t value);
void MBC1Kill();


#endif // MBC1_H
