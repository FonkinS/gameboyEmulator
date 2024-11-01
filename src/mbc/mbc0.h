#ifndef MBC0_H
#define MBC0_H
#include <stdint.h>
#include <stdlib.h>

void MBC0Init(uint8_t *data, long long length);
uint8_t MBC0Read(uint16_t index);
void MBC0Write(uint16_t index, uint8_t value);
void MBC0Kill();

uint8_t *rom;
uint8_t *ram;


#endif
