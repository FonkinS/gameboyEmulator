#ifndef INPUT_H
#define INPUT_H

enum BUTTON {BUTTON_A, BUTTON_B, BUTTON_START, BUTTON_SELECT, BUTTON_UP, BUTTON_DOWN, BUTTON_LEFT, BUTTON_RIGHT, BUTTON_SPECIAL};

enum PRESS_TYPE {RELEASE, PRESS};



void inputSetCallback(void (*callback)(enum BUTTON, enum PRESS_TYPE));

void inputInit();
void inputTick(void* null); // PASS NULL TO THIS

void inputClearCallback();



#endif
