// COMP2521 Assignment 2

// Written by:
// Date:

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graph.h>
#include <queue.h>
#define BUFF_SIZE 256

char *return_strcat(char *line,const char* append_txt){
    char *str = malloc(strlen(line)+strlen(append_txt+1));
    strcat(str,line);
    strcat(str,append_txt);
    return str;
}

Queue Q_URLS(char *line,Queue q){
		char *url_str;
		const char s[2] = " ";
		char *token;
		token = strtok(line, s);
		
		//loop separating path via / delimiter
		while(token != NULL ) {
            if(token != NULL){
				char *url_str = return_strcat(line,".txt");
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
    Queue url_q = QueueNew;
    char str[BUFF_SIZE];
    fp = fopen(filename, "r");
    if(fp == NULL){
        fprintf(stderr,"File pointer is NULL.\n")
        exit(EXIT_FAILURE);
    }
    char * ret;
    bool section_ended = 0;
    while(!section_ended){
        ret = gets(line,BUFF_SIZE,fp);
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
    return q;
}

int main(int argc, char *argv[]) {
    int damp_factor, diff_pr, max_iterations;
    damp_factor = atoi(argv[1]);
    diff_pr = atoi(argv[2]);
    max_iterations = atoi(argv[3]);

    //skip line and EOF are dummy strings
    Queue collection_q = get_urls("collection.txt","skip line","EOF");

    //make graph size of collection q the queue index is the vertex number
    Graph url_graph = GraphNew(collection_q->size);

    int iteration = 0;
    int diff = diff_pr;
    Node curr;
    int idx;
    int weight;
    while(iteration < max_iterations && diff >= diff_pr){
        curr = collection_q->head;
        idx = 0
        if(iteration == 0){
            while(curr != NULL){
                //explore each url make edges and assign weight to be 1/N
                weight = 1/q->size ;
                add_edges_from_url(url_graph,idx,weight);
                idx++;
                curr = curr->next;
            }
            
        }
        else{
            while(curr != NULL){
                //explore each url make edges and assign weight to be PR(pi,t+1)=1−dN+d∗∑pj∈M(pi)PR(pj,t)∗Win(pj,pi)∗Wout(pj,pi)
                weight = 
                add_edges_from_url(url_graph,idx,weight);
                idx++;
                curr = curr->next;
            }
        }
        iteration++;
    }

    //navigate through queue in a while loop until reach end of collection queue
    QueueFree(collection_q);
}

