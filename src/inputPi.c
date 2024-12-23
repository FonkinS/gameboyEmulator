#include "inputPi.h"
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


void joypadTick(void* null) {
	button_a = digitalRead(BUTTON_A) == LOW;
	button_b = digitalRead(BUTTON_B) == LOW;
	button_start = digitalRead(BUTTON_START) == LOW;
	button_select = digitalRead(BUTTON_SELECT) == LOW;
	dpad_up = digitalRead(DPAD_UP) == LOW;
	dpad_down = digitalRead(DPAD_DOWN) == LOW;
	dpad_left = digitalRead(DPAD_LEFT) == LOW;
	dpad_right = digitalRead(DPAD_RIGHT) == LOW;
}
