#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Queue.h"


//return a concatenated string
char *return_strcat(char *line,const char* append_txt);

//put all urls in a line of input into a queue
Queue Q_URLS(char *line,Queue q);

/*get the urls from the filename file where the urls exist between some start string
 and some end string
*/
Queue get_urls(const char * filename,const char* start, const char* end);