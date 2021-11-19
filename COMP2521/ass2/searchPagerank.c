// COMP2521 Assignment 2

// Written by: Jake Edwards 
// Date: 19/11/2021

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"
#include "readData.h"
int main(int argc, char *argv[]) {
    char **queryWords = malloc(argc * sizeof(char *));
    assert(queryWords != NULL);
    // int N = argc -1;
    for(int i = 1;i < argc;i++){
        queryWords[i-1] = malloc(sizeof(char)*(strlen(argv[i])+1));
        assert(queryWords[0] != NULL);
        strcpy(queryWords[i-1],argv[i]);
    }
    // List matchedUrlList = findMatchedUrls("invertedIndex.txt",queryWords);

}

