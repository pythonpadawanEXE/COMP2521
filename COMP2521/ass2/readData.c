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
char *returnStrcat(char *line,const char* appendTxt){
    char *str = malloc(strlen(line)+strlen(appendTxt+1));
    strcat(str,line);
    strcat(str,appendTxt);
    return str;
}

//put all urls in a line of input into a queue
Queue qUrls(char *line,Queue q){
        

        line[strcspn(line, "\r\n")] = 0;

		const char s[2] = " ";
		char *token;
        
		token = strtok(line, s);
		
		//loop separating path via / delimiter
		while(token != NULL ) {
            if(token != NULL && strcmp(token,"\n") != 0){

				QueueEnqueue(q,token);
				
            }
			token = strtok(NULL, s);
		}
        return q;
  
}


/*get the urls from the filename file where the urls exist between some start string
 and some end string
*/
Queue getUrls(const char * filename,const char* start, const char* end){
    FILE *fp;
    Queue urlQ = QueueNew();
    fp = fopen(filename, "r");
    if(fp == NULL){
        fprintf(stderr,"File pointer is NULL.\n");
        exit(EXIT_FAILURE);
    }
    size_t SIZE = 0;
    char *line = NULL;
    bool sectionEnded = false;
    int ret = 0;

    //char URL[MAX_URL_LENGTH];

    while(!sectionEnded){
        //ret = fscanf(fp,"%s",URL);
        ret = getline(&line,&SIZE,fp);
        if(ret == -1){
            sectionEnded = true;
            
            break;
        }
        else if(strcmp(line,end) == 0){
            sectionEnded = true;
            
            break;
        }
        else if(strcmp(line,start) == 0 || strcmp("\n",line) == 0){
            
            continue;
        }

       
        qUrls(line,urlQ);
        
    }
    free(line);
    fclose(fp);
    return urlQ;
}

List findMatchedUrls(const char* filename,char ** queryWords){
    return NULL;
}