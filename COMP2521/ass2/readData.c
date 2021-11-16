#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Graph.h"
#include "Queue.h"
#include "readData.h"


//return a concatenated string
char *return_strcat(char *line,const char* append_txt){
    char *str = malloc(strlen(line)+strlen(append_txt+1));
    strcat(str,line);
    strcat(str,append_txt);
    return str;
}

//put all urls in a line of input into a queue
Queue Q_URLS(char *line,Queue q){
		//char *url_str;
		const char s[2] = " ";
		char *token;
		token = strtok(line, s);
		
		//loop separating path via / delimiter
		while(token != NULL ) {
            if(token != NULL){
				//char *url_str = return_strcat(line,".txt");
				QueueEnqueue(q,line);
				
            }
			token = strtok(NULL, s);
		}
		return q;   
}


/*get the urls from the filename file where the urls exist between some start string
 and some end string
*/
Queue get_urls(const char * filename,const char* start, const char* end){
    FILE *fp;
    Queue url_q = QueueNew();
    fp = fopen(filename, "r");
    if(fp == NULL){
        fprintf(stderr,"File pointer is NULL.\n");
        exit(EXIT_FAILURE);
    }
    char * ret;
    bool section_ended = 0;
    char *line = NULL;
    while(!section_ended){
        ret = fgets(line,BUFF_SIZE,fp);
        if(strcmp(line,end) == 0 || ret == NULL){
            section_ended = 1;
            break;
        }
        else if(strcmp(line,start) == 0){
            continue;
        }
        Q_URLS(line,url_q);
    }
    fclose(fp);
    return url_q;
}