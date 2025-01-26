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
