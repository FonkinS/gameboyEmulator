#ifndef MBC1_H
#define MBC1_H
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

void MBC1Init(uint8_t *data, long long length, bool load_save_data);
uint8_t MBC1Read(uint16_t index);
void MBC1Write(uint16_t index, uint8_t value);

void MBC1SaveData(char* filename);

void MBC1Kill();


void MBC1LoadData(char* filename);


#endif // MBC1_H
