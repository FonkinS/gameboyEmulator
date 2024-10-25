#ifndef JOYPAD_H
#define JOYPAD_H
#include <stdint.h>
#include <stdbool.h>

uint8_t joypad_read();
void joypad_write(uint8_t value);

bool buttons_selected;
bool dpad_selected;
bool start_down;
bool select_up;
bool b_left;
bool a_right;





#endif
