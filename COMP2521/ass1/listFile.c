// COMP2521 Assignment 1

// !!! DO NOT MODIFY THIS FILE !!!

#include <stdio.h>
#include <stdlib.h>

#include "FileType.h"

#define BLUE "\033[94m"
#define RESET_COLOR "\033[0m"

void listFile(char *name, FileType type) {
#ifdef COLORED
    char *color = RESET_COLOR;
    switch (type) {
        case REGULAR_FILE:  color = RESET_COLOR; break;
        case DIRECTORY:     color = BLUE;        break;
        default:            color = RESET_COLOR; break;
    }
    printf("%s%s%s", color, name, RESET_COLOR);
#else
    printf("%s", name);
#endif
}

