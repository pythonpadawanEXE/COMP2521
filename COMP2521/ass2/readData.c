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
        
        //line[strlen(line)+1] = ' ';
        line[strcspn(line, "\r\n")] = 0;
		//char *url_str;
		const char s[2] = " ";
		char *token;
        
		token = strtok(line, s);
		
		//loop separating path via / delimiter
		while(token != NULL ) {
            if(token != NULL && strcmp(token,"\n") != 0){
				//char *url_str = return_strcat(line,".txt");
				QueueEnqueue(q,token);
				
            }
			token = strtok(NULL, s);
		}
        return q;

        // char URL[MAX_URL_LENGTH];
        // int ret;
        // bool line_ended = false;
        // while(!line_ended){
        //     ret = sscanf(line,"%s",URL);
        //     if(ret == -1){
        //         line_ended = true;
        //         break;
        //     }
        //     if(strcmp(URL," ") != 0 || ){
        //         line = &line[strlen(URL)+1];
        //         QueueEnqueue(q,URL);
        //     }
        // }
		// return q;   
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
    size_t SIZE = 0;
    char *line = NULL;
    bool section_ended = false;
    int ret = 0;

    //char URL[MAX_URL_LENGTH];

    while(!section_ended){
        //ret = fscanf(fp,"%s",URL);
        ret = getline(&line,&SIZE,fp);
        if(ret == -1){
            section_ended = true;
            
            break;
        }
        else if(strcmp(line,end) == 0){
            section_ended = true;
            
            break;
        }
        else if(strcmp(line,start) == 0 || strcmp("\n",line) == 0){
            
            continue;
        }
        //QueueEnqueue(url_q,URL);
       
        Q_URLS(line,url_q);
        
    }
    free(line);
    fclose(fp);
    return url_q;
}