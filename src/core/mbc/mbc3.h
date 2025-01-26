/*
 * Copyright (C) 2025 Aaron Kainu
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/



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
