#include "inputDesktop.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

void inputInit() {
    // Does nothing, virtual
}

void inputTick(void* window) {
    button_a = glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS;
    button_b = glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS;
    button_start = glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS;
    button_select = glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS;
    dpad_up = glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS;
    dpad_down = glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS;
    dpad_left = glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS;
    dpad_right = glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS;
}
