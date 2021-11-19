#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"
#include "Queue.h"


//return a concatenated string
char *returnStrcat(char *line,const char* appendTxt);

//put all urls in a line of input into a queue
Queue qUrls(char *line,Queue q);

/*get the urls from the filename file where the urls exist between some start string
 and some end string
*/
Queue getUrls(const char * filename,const char* start, const char* end);

List findMatchedUrls(const char* filename,char ** queryWords);