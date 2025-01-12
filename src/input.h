#ifndef INPUT_H
#define INPUT_H

/* There exists no input.c file in this directory. Instead these prototypes are implemented by pi/inputPi.c
 * and Desktop/inputDesktop.c, depending on what platform is used. The system works by being given a callback 
 * function upstream, which is then called when one of the BUTTON buttons are pressed */

enum BUTTON {BUTTON_A, BUTTON_B, BUTTON_START, BUTTON_SELECT, BUTTON_UP, BUTTON_DOWN, BUTTON_LEFT, BUTTON_RIGHT, BUTTON_SPECIAL};

enum PRESS_TYPE {RELEASE, PRESS};



void inputSetCallback(void (*callback)(enum BUTTON, enum PRESS_TYPE));

void inputInit();
void inputTick(void* null); // PASS NULL TO THIS

void inputClearCallback();



#endif
