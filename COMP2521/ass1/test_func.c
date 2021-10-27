#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
int main () {
   char str[80] = "/(root)/bin/ls";
   const char s[2] = "/";
   char *token;
   
   printf("%s","/" str);
   // get the first token 
   //token = strtok(str, s);
   
   // walk through other tokens 
   //while( token != NULL ) {
      printf( " %s\n", token );
      
      token = strtok(NULL, s);
   }
   
   return(0);
}
*/
void prepend(char* s, const char* t);

/* Prepends t into s. Assumes s has enough space allocated
** for the combined string.
*/
void prepend(char* s, const char* t)
{
    size_t len = strlen(t);
    memmove(s + len, s, strlen(s) + 1);
    memcpy(s, t, len);
}

int main()
{
    char* s = malloc(100);
    strcpy(s, "file");
    prepend(s, "dir/");

    printf("%s\n", s);
    return 0;
}