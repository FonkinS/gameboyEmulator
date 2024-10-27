#ifndef JOYPAD_H
#define JOYPAD_H
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

uint8_t joypad_read();
void joypad_write(uint8_t value);

bool buttons_selected;
bool dpad_selected;

bool button_start;
bool dpad_down;
bool button_select;
bool dpad_up;
bool button_b;
bool dpad_left;
bool button_a;
bool dpad_right;





#endif
