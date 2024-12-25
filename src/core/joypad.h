#ifndef JOYPAD_H
#define JOYPAD_H
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "../input.h"

void joypadInit();
uint8_t joypadRead();
void joypadWrite(uint8_t value);

void joypadInputCallback(enum BUTTON button, enum PRESS_TYPE type);

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
