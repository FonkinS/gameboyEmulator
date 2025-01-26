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
