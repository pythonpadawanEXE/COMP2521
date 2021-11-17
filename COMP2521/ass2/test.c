#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void){
    char src[40];
    char dest[100];
    char dest2[100];

    strcpy(src, "This is tutorialspoint.com");
    strcpy(dest, src);
    strcpy(src,"did i trick u?");
    printf("%s\n%s\n",src,dest);
    sscanf(dest,"%s",dest2);
    printf("%s",dest);

    return 0;
}