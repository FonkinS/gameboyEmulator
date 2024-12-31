#ifndef MBC3_H
#define MBC3_H
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>



void MBC3Init(uint8_t *data, long long length, bool load_data);
uint8_t MBC3Read(uint16_t index);
void MBC3Write(uint16_t index, uint8_t value);
void MBC3Kill();


void MBC3SaveData(char* filename);

uint8_t MBC3GetRTC();

void MBC3LoadData(char* filename);




#endif // MBC3_H
