#include "ppu.h"
//#include "../include/sgf.h"
#include "joypad.h"
#include "lcd.h"
#include <stdint.h>
#include <stdio.h>

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>


GLFWwindow *window;
uint8_t screen[160*144];
uint8_t faux_bg_screen[160*144];

unsigned int shader;
unsigned int vao;
unsigned int texture;


const char* vertexShader = "#version 330 core\n" \
                           "layout (location = 0) in vec3 aPos;\n" \
                           "layout (location = 1) in vec2 aTexCoord;\n" \
                           "uniform vec2 pos;\n" \
                           "out vec2 texCoord;\n" \
                           "void main(){\n" \
                           "\tgl_Position = vec4(aPos.x + pos.x, aPos.y + pos.y, aPos.z, 1.0);\n" \
                           "\ttexCoord = aTexCoord;\n" \
                           "}\n";


const char* fragmentShader = "#version 330 core\n" \
                            "out vec4 FragColor;\n" \
                            "in vec2 texCoord;\n" \
                            "uniform sampler2D inTexture;\n" \
                            "uniform vec3 colors[4];\n" \
                            "void main() {\n" \
                            "\tint col = int(texture(inTexture, texCoord).r*256);\n" \
                            "\tFragColor = vec4(colors[col],1.0f);\n" \
                            "}\n";



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int PPUInit(char* title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    window = glfwCreateWindow(640, 576, title, NULL, NULL);

    glfwMakeContextCurrent(window);


    float vertices[] = {
        1.0, -1.0, 0.0f, 1.0f, 1.0f,
        1.0,1.0, 0.0f, 1.0f, 0.0f,
        -1.0,1.0, 0.0f, 0.0f, 0.0f,
        -1.0, -1.0, 0.0f, 0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 20*sizeof(float), vertices, GL_STATIC_DRAW);
    unsigned int ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(float), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    
    glBindVertexArray(0);
    

    unsigned int vs;
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShader, NULL);
    glCompileShader(vs);
    int success;
    char error_log[512];
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vs, 512, NULL, error_log);
        printf("ERROR IN VERTEX SHADER:\n%s\n", error_log);
    }


    unsigned int fs;
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShader, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fs, 512, NULL, error_log);
        printf("ERROR IN FRAGMENT SHADER:\n%s\n", error_log);
    }

    shader = glCreateProgram();
    glAttachShader(shader, vs);
    glAttachShader(shader, fs);
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader, 512, NULL, error_log);
        printf("ERROR IN SHADER COMPILE:\n%s\n", error_log);
    }

    glDeleteShader(vs);
    glDeleteShader(fs);


    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, 160,144, 0, GL_RED, GL_UNSIGNED_BYTE, screen);
    glGenerateMipmap(GL_TEXTURE_2D);

    glfwSetKeyCallback(window, key_callback);
    
    float colors[4][3] = {{0.79, 0.86, 0.58}, {0.54, 0.63, 0.41}, {0.26, 0.37, 0.24}, {0.05, 0.09, 0.07}};
    glUseProgram(shader);
    glUniform3fv(glGetUniformLocation(shader, "colors"), 4, (float*)colors);

    glfwSwapInterval(0);
    glBindVertexArray(vao);
    return 1;
}

int renderFrame() {
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 160, 144, GL_RED, GL_UNSIGNED_BYTE, screen);
    
    glBindTexture(GL_TEXTURE_2D, texture);
    glUseProgram(shader);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();

    return !glfwWindowShouldClose(window);
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if      (key == GLFW_KEY_UP) dpad_up = action == GLFW_RELEASE ? false : true;
    else if (key == GLFW_KEY_LEFT) dpad_left = action == GLFW_RELEASE ? false : true;
    else if (key == GLFW_KEY_DOWN) dpad_down = action == GLFW_RELEASE ? false : true;
    else if (key == GLFW_KEY_RIGHT) dpad_right = action == GLFW_RELEASE ? false : true;
    else if (key == GLFW_KEY_BACKSPACE) button_select = action == GLFW_RELEASE ? false : true;
    else if (key == GLFW_KEY_ENTER) button_start = action == GLFW_RELEASE ? false : true;
    else if (key == GLFW_KEY_Z) button_b = action == GLFW_RELEASE ? false : true;
    else if (key == GLFW_KEY_X) button_a = action == GLFW_RELEASE ? false : true;
    else if (key == GLFW_KEY_Q) exit(0);
}




void PPUKill() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

// TODO Notes FOr TOmorrow
// - Table of which objects visible
// - STRUCTURE for objects
int frame = 0;
void drawScanline(int scanline, int vsync) {
    if (scanline == 0) frame++;
    if ((frame+scanline) % vsync != 0) return;
    if (BGWinEnable) {
        // TODO Fix scrolling edges
        uint8_t y = (scanline + SCY) % 256;
        uint8_t tiley = y / 8;
        uint8_t ty = (scanline + SCY) % 8; 
        for (int tilex = 0; tilex < 32; tilex++) {
            uint8_t index = BusRead(BGTileMap + tilex + tiley*32);
            int tile = BGWinTileData + (int)(BGWinTileData == 0x8000 ? (uint8_t)index : (int8_t)index) * 16;
            uint8_t first = BusRead(tile + ty*2);
            uint8_t second = BusRead(tile + ty*2+1);
            for (int tx = 0; tx < 8; tx++) {
                uint8_t x = (tilex * 8 + tx + SCX) % 256;
                if (x < 0 || x >= 160) continue;
                screen[scanline*160+x] = BGP[((first >> (7-tx)) & 1) + (((second >> (7-tx)) & 1) << 1)];
            }
        }

        if (WinEnable && WX >= 0 && WX <= 166 && WY >= 0 && WY <= 143) {
            uint8_t tiley = y / 8;
            uint8_t ty = (scanline + WY) % 8; 
            for (int tilex = WX / 8; tilex < 21 + WX / 8; tilex++) {
                uint8_t index = BusRead(WinTileMap + tilex + tiley*32);
                int tile = BGWinTileData + (int)(BGWinTileData == 0x8000 ? (uint8_t)index : (int8_t)index) * 16;
                uint8_t first = BusRead(tile + ty*2);
                uint8_t second = BusRead(tile + ty*2+1);
                for (int tx = 0; tx < 8; tx++) {
                    uint8_t wx = tilex * 8 + tx;
                    if (wx < 0 || wx >= 160) continue;
                    screen[scanline*160+wx] = BGP[((first >> (7-tx)) & 1) + (((second >> (7-tx)) & 1) << 1)];
                }
            }
        }
    }     

    // TODO Support for Sprite Flipping
    // TODO Support For 8x16 sprites
    // TODO Pokemon Blue long blank screen pauses?
    // Objects
    if (OBJEnable) {
        for (int o = 0xfe00; o < 0xfea0; o+=4) {
            uint8_t y = BusRead(o)-16;
            uint8_t x = BusRead(o+1);
            if (scanline-y < OBJSize && scanline >= y && x != 0 && x < 168) {
                uint8_t tile = BusRead(o+2);
                uint8_t flags = BusRead(o+3);
                uint8_t act_y = scanline-y;

                uint8_t first = BusRead(0x8000 + tile*16 + act_y * 2);
                uint8_t second = BusRead(0x8000 + tile*16 + act_y * 2 + 1);
                
                uint8_t *obp = (flags & 0x10) ? OBP1 : OBP0;
                for (int ox = 0; ox < 8; ox++) {
                    if (ox+x < 0) continue;
                    if (ox+x >= 168) break;
                    if (flags & 0x80 && faux_bg_screen[scanline*160+ox+x-8] > 0) continue; // BG Priority
                    uint8_t color;
                    if (flags & 0x20) color = (((first >> ((ox%8))) & 1)) + (((second >> ((ox%8))) & 1) << 1); // X Flip
                    else color = (((first >> (7-(ox%8))) & 1)) + (((second >> (7-(ox%8))) & 1) << 1);
                    if (color == 0) continue;
                    screen[scanline * 160 + ox+x-8] = obp[color-1];
                    }
                }
            }
    }
}



