#include "menu.h"
#include "font.h"

#include <dirent.h>
#include <stdlib.h>
#include <stdbool.h>

void menuInit(const char* fontFile) {
    fontInit(fontFile);

    struct dirent *files;
    DIR *dir = opendir("games/");
    if (dir == NULL){
        printf("Directory cannot be opened!" );
        return;
    }
    int i = 0;
    while ((files = readdir(dir)) != NULL) {
        char* filename = (char*) calloc(32, sizeof(char));
        bool valid = false;
        for (int j = 0; j < 32; j++) {
            if (files->d_name[j] == '\0') {break;}
            if (files->d_name[j] == '.' && files->d_name[j+1] == 'g') {valid = true; break;}
            filename[j] = files->d_name[j];
        }
        if (valid) renderLine(10,i++*10+10, filename, 3);
    }
    closedir(dir);
    //renderMultiline(10,10,150,134,"Hello World", 3, 0); 
   //renderChar(0,0,'C', 3);
}

void menuKill() {
    fontKill();
}
