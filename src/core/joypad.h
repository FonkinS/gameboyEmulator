#ifndef JOYPAD_H
#define JOYPAD_H
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

uint8_t joypad_read();
void joypad_write(uint8_t value);

enum JoypadIO {
    rJOY = 0xff00
};


extern bool button_start;
extern bool dpad_down;
extern bool button_select;
extern bool dpad_up;
extern bool button_b;
extern bool dpad_left;
extern bool button_a;
extern bool dpad_right;





#endif
