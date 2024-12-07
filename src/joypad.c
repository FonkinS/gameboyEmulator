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

#ifdef __linux__
#include <wiringPi.h>
#define BUTTON_A 14
#define BUTTON_B 15
#define BUTTON_START 0
#define BUTTON_SELECT 5
#define DPAD_UP 6
#define DPAD_DOWN 13
#define DPAD_LEFT 19
#define DPAD_RIGHT 26

void joypadInit() {
	wiringPiSetupGpio();
	pinMode(BUTTON_A, INPUT);
	pinMode(BUTTON_B, INPUT);
	pinMode(BUTTON_START, INPUT);
	pinMode(BUTTON_SELECT, INPUT);
	pinMode(DPAD_UP, INPUT);
	pinMode(DPAD_DOWN, INPUT);
	pinMode(DPAD_LEFT, INPUT);
	pinMode(DPAD_RIGHT, INPUT);
	pullUpDnControl(BUTTON_A, PUD_UP);
	pullUpDnControl(BUTTON_B, PUD_UP);
	pullUpDnControl(BUTTON_START, PUD_UP);
	pullUpDnControl(BUTTON_SELECT, PUD_UP);
	pullUpDnControl(DPAD_UP, PUD_UP);
	pullUpDnControl(DPAD_DOWN, PUD_UP);
	pullUpDnControl(DPAD_LEFT, PUD_UP);
	pullUpDnControl(DPAD_RIGHT, PUD_UP);
}


void joypadTick() {
	button_a = digitalRead(BUTTON_A) == LOW;
	button_b = digitalRead(BUTTON_B) == LOW;
	button_start = digitalRead(BUTTON_START) == LOW;
	button_select = digitalRead(BUTTON_SELECT) == LOW;
	dpad_up = digitalRead(DPAD_UP) == LOW;
	dpad_down = digitalRead(DPAD_DOWN) == LOW;
	dpad_left = digitalRead(DPAD_LEFT) == LOW;
	dpad_right = digitalRead(DPAD_RIGHT) == LOW;
}

#else

void joypadInit() {
// DOES NOTHING, ON DESKTOP THIS STUFF IS IN PPU.C
}

void joypadTick() {
// DOES NOTHING, ON DESKTOP THIS STUFF IS IN PPU.C
}

#endif

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
