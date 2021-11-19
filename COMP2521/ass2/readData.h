#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"
#include "Queue.h"

typedef struct searchItem{
    char *URL;
    double pageRank;
    int numMatches;
}SearchItem;


//return a concatenated string
char *returnStrcat(char *line,const char* appendTxt);

//put all urls in a line of input into a queue
Queue qUrls(char *line,Queue q);

/*get the urls from the filename file where the urls exist between some start string
 and some end string
*/
Queue getUrls(const char * filename,const char* start, const char* end);

SearchItem* findMatchedUrls(const char* filename,char ** queryWords);
void ArrayListShow(struct searchItem* UrlList);
void ArrayFree(SearchItem * List);