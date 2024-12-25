#include "../input.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdbool.h>

void (*callback)(enum BUTTON, enum PRESS_TYPE);
bool callback_set = false;

bool prev_a = false;
bool prev_b = false;
bool prev_start = false;
bool prev_select = false;
bool prev_up = false;
bool prev_down = false;
bool prev_left = false;
bool prev_right = false;




void inputInit() {
    // Does nothing, virtual
}

void inputSetCallback(void (*c)(enum BUTTON, enum PRESS_TYPE)) {
    callback_set = true;
    callback = c;
}

void inputTick(void* window) {
    if (callback_set) {
        if ((glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) != prev_a) {(callback)(BUTTON_A, !prev_a); prev_a = !prev_a;}
        if ((glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) != prev_b) {(callback)(BUTTON_B, !prev_b); prev_b = !prev_b;} 
        if ((glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) != prev_start) {(callback)(BUTTON_START, !prev_start); prev_start = !prev_start;} 
        if ((glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) != prev_select) {(callback)(BUTTON_SELECT, !prev_select); prev_select = !prev_select;} 
        if ((glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) != prev_up) {(callback)(BUTTON_UP, !prev_up); prev_up = !prev_up;} 
        if ((glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) != prev_down) {(callback)(BUTTON_DOWN, !prev_down); prev_down = !prev_down;} 
        if ((glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) != prev_left) {(callback)(BUTTON_LEFT, !prev_left); prev_left = !prev_left;} 
        if ((glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) != prev_right) {(callback)(BUTTON_RIGHT, !prev_right); prev_right = !prev_right;} 

    }
}

void inputClearCallback() {
    callback_set = false;
}
