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



#ifndef MBC0_H
#define MBC0_H
#include <stdint.h>
#include <stdlib.h>

void MBC0Init(uint8_t *data, long long length);
uint8_t MBC0Read(uint16_t index);
void MBC0Write(uint16_t index, uint8_t value);
void MBC0Kill();


#endif
