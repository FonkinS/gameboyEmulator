/*
 * Copyright (C) 2025 Aaron Kainu
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "../render.h"

#include <stdio.h>

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>



GLFWwindow *window;
unsigned int shader;
unsigned int vao;
unsigned int texture;


/* The OpenGL API relies on user made shaders. Here only two are used, vertex and fragment*/

// The vertexShader takes in 2 layout variables, the pixels position in the window (each axis
// between -1.0 and 1.0), and the pixel's position in the texture (each axis between 0 and 1)
// The texture position is passed on to the fragment shader, and outputs the screen position
const char* vertexShader = "#version 330 core\n" \
                           "layout (location = 0) in vec3 aPos;\n" \
                           "layout (location = 1) in vec2 aTexCoord;\n" \
                           "uniform vec2 pos;\n" \
                           "out vec2 texCoord;\n" \
                           "void main(){\n" \
                           "\tgl_Position = vec4(aPos.x + pos.x, aPos.y + pos.y, aPos.z, 1.0);\n" \
                           "\ttexCoord = aTexCoord;\n" \
                           "}\n";

// The fragment shader takes in the texture position, then samples the texture given to it at that
// position, and uses that color index to find the actual color in the colors[4] array.
const char* fragmentShader = "#version 330 core\n" \
                            "out vec4 FragColor;\n" \
                            "in vec2 texCoord;\n" \
                            "uniform sampler2D inTexture;\n" \
                            "uniform vec3 colors[4];\n" \
                            "void main() {\n" \
                            "\tint col = int(texture(inTexture, texCoord).r*256);\n" \
                            "\tFragColor = vec4(colors[col],1.0f);\n" \
                            "}\n";




/* The following function looks really complicated, but all it does is
 * initialise glfw, 
 * construct the OpenGL Object which rendering will be performed to, 
 * give it the two shaders defined above, and 
 * give it an OpenGL Texture which will be changed later*/
int renderInit(char* title) {
    // Init GLFW and the window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    window = glfwCreateWindow(640, 576, title, NULL, NULL);
    glfwMakeContextCurrent(window);


    // Create the vertices for the screen texture
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

    // Create the OpenGL Objects (Vertex, Element and Buffer) which define the screen texture
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

    // Pass the vertices to the OpenGL Objects
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // Instantiate the Vertex Shader
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


    // Instantiate the Fragment Shader
    unsigned int fs;
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShader, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fs, 512, NULL, error_log);
        printf("ERROR IN FRAGMENT SHADER:\n%s\n", error_log);
    }

    // Create the Shader Program for the OpenGL Object
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


    // Creat the Texture Object for the screen
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

    // Sets the color palette uniform for the fragment shader
    float colors[4][3] = {{0.79, 0.86, 0.58}, {0.54, 0.63, 0.41}, {0.26, 0.37, 0.24}, {0.05, 0.09, 0.07}};
    glUseProgram(shader);
    glUniform3fv(glGetUniformLocation(shader, "colors"), 4, (float*)colors);

    // Disables VSync, which speeds up rendering (otherwise there are occasional framedrops for some reason)
    glfwSwapInterval(0);

    return 1;
}

int renderFrame() {
    // Updates the OpenGL Texture to match the screen[] virtualisation
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 160, 144, GL_RED, GL_UNSIGNED_BYTE, screen);
    
    // Draw the OpenGL Texture object
    glBindTexture(GL_TEXTURE_2D, texture);
    glUseProgram(shader);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();

    inputTick(window);

    // Tells the main loop whether or not to quit
    return !glfwWindowShouldClose(window);
}

void renderKill() {
    glfwDestroyWindow(window);
    glfwTerminate();
}
