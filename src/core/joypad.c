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

void (*endcallback)();

void joypadInit(void (*ec)()) {
    button_a = false;
    button_b = false;
    button_start = false;
    button_select = false;
    dpad_up = false;
    dpad_down = false;
    dpad_right = false;
    dpad_left = false;
	endcallback = ec;
    inputSetCallback(joypadInputCallback);
}

uint8_t joypadRead() {
    if (!buttons_selected && !dpad_selected) return 0xff;
    else if (buttons_selected) {
        return 0xe0 + (!button_start << 3) + (!button_select << 2) + (!button_b << 1) + !button_a;
    } else if (dpad_selected) {
        return 0xd0 + (!dpad_down << 3) + (!dpad_up << 2) + (!dpad_left << 1) + !dpad_right;
    }
    return 0;
}


void joypadWrite(uint8_t value) {
    value = ~value;
    buttons_selected = false;
    dpad_selected = false;
    if (value & 32) buttons_selected = true;
    else if (value & 16) dpad_selected = true;
} 


// Will be called by inputPi.c or inputDesktop.c whenever one of the BUTTON enum buttons are pressed or released
void joypadInputCallback(enum BUTTON button, enum PRESS_TYPE type) {
    if (button == BUTTON_A) button_a = type;
    if (button == BUTTON_B) button_b = type;
    if (button == BUTTON_START) button_start = type;
    if (button == BUTTON_SELECT) button_select = type;
    if (button == BUTTON_UP) dpad_up = type;
    if (button == BUTTON_DOWN) dpad_down = type;
    if (button == BUTTON_LEFT) dpad_left = type;
    if (button == BUTTON_RIGHT) dpad_right = type;
    if (button == BUTTON_SPECIAL && type == PRESS) {(endcallback)();} 
}
