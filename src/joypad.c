#include "joypad.h"

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

uint8_t joypad_read() {
    if (!buttons_selected && !dpad_selected) return 0xff;
    else if (buttons_selected) {
        //printf("b\n");
        return 0xe0 + (!button_start << 3) + (!button_select << 2) + (!button_b << 1) + !button_a;
    } else if (dpad_selected) {
        //printf("d\n");
        return 0xd0 + (!dpad_down << 3) + (!dpad_up << 2) + (!dpad_left << 1) + !dpad_right;
    }
    return 0;
}


void joypad_write(uint8_t value) {
    value = ~value;
    buttons_selected = false;
    dpad_selected = false;
    if (value & 32) buttons_selected = true;
    else if (value & 16) dpad_selected = true;
} 
