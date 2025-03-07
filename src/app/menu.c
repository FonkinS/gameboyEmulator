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

#include "menu.h"
#include "font.h"

#include <dirent.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

bool started = false;

void (*gamestartcallback)(char*);

int cursor_pos = 0;
int list_length = 0;
char** gamePaths;
char thisPath[64];

void menuInit(void (*c)()) {
    inputSetCallback(menuInputCallback);
    gamestartcallback = c;

	list_length = 0;
    fontInit("assets/font.bin");

	for (int y = 0; y < 144; y++) {
		for (int x = 0; x < 160; x++) {
			screen[y*160+x] = 0;
		}
	}

    struct dirent *files;
    DIR *dir = opendir("games/");
    if (dir == NULL){
        printf("Directory cannot be opened!" );
        return;
    }
    int i = 0;
    gamePaths = (char**) calloc(16, sizeof(char*));
    for (int j = 0; j < 16; j++) {
        gamePaths[j] = (char*) calloc(64, sizeof(char));
    }
    while ((files = readdir(dir)) != NULL) {
        int len = strlen(files->d_name);
        char* filename = (char*) calloc(32, sizeof(char));
        bool valid = false;
        for (int j = 0; j < 32; j++) {
            if (files->d_name[j] == '\0') {break;}
            if (files->d_name[j] == '.' && files->d_name[j+1] == 'g' && j+3 == len) {valid = true; break;}
            filename[j] = files->d_name[j];
        }
        if (valid) {
            renderLine(16,i++*12+10, filename, 3);
            for (int j = 0; j < 32; j++) {
                if (files->d_name[j] == '\0') {break;}
                gamePaths[list_length][j] = files->d_name[j];
            }
            list_length++;
        }
        free(filename);
    }
    closedir(dir);
    placeCursor(0);
}

void menuTick() {
}

void menuKill() {
    for (int i = 0; i < 16; i++) {
        free(gamePaths[i]);
    }
    free(gamePaths);
    fontKill();
}

void placeCursor(int index) {
    cursor_pos = index;
    if (cursor_pos < 0) cursor_pos = 0;
    else if (cursor_pos >= list_length) cursor_pos = list_length-1;
    for (int y = 0; y < 144; y++) {
        for (int x = 0; x < 16; x++) {
            screen[y*160+x] = 0;
        }
    }
    renderChar(4, cursor_pos*12+10, 'x', 3);
}

void menuInputCallback(enum BUTTON button, enum PRESS_TYPE type) {
    if (button == BUTTON_START && type == PRESS) {
        strcpy(thisPath, "games/");
        strcat(thisPath, gamePaths[cursor_pos]);
        (gamestartcallback)(thisPath);
    }
    else if (button == BUTTON_UP && type == PRESS) placeCursor(cursor_pos-1);
    else if (button == BUTTON_DOWN && type == PRESS) placeCursor(cursor_pos+1);
}
