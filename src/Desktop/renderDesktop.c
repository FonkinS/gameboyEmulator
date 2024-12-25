#include "../render.h"

#include <stdint.h>
#include <stdio.h>

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>


uint8_t screen[160*144];


GLFWwindow *window;
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




int renderInit(char* title) {
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

    inputTick(window);

    return !glfwWindowShouldClose(window);
}

void renderKill() {
    glfwDestroyWindow(window);
    glfwTerminate();
}
