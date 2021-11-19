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
List getList(const char *filename){
    FILE *fp = fopen(filename, "r");
    if(fp == NULL){
        fprintf(stderr,"File pointer is NULL.\n");
        exit(EXIT_FAILURE);
    }

    char *Line = NULL;
    char URL[MAX_URL_LENGTH];
    double pageRank;
    List pageRankList = ListNew();
    int ret = 0;
    bool inputEnded = false;
    size_t SIZE = 0;
    while(!inputEnded){
        ret = getline(&Line,&SIZE,fp);
        if(ret < 0){
            inputEnded = true;
            break;
        } 
        ret = sscanf(Line,"%s %*s %lf",URL,&pageRank);
        URL[strcspn(URL, ",")] = 0;
        if(ret < 0) break;
        ListAppend(pageRankList,pageRank,URL);
    }
    free(Line);
    fclose(fp);
    return pageRankList;
}

bool issearchWordinListArray(char ** queryWords,char * searchWord){
    for(int i = 0; queryWords[i] != NULL;i++){
        if(strcmp(queryWords[i],searchWord)==0)return true;
    }
    return false;
}
int isStringInListArray(char *str,SearchItem* UrlList){
    int i;
    for(i=0;UrlList[i].URL != NULL;i++){
        if(strcmp(UrlList[i].URL,str)==0){
            return -1;
        }
    }
    return i;
}
void AddLineContents(Queue lineContents,struct searchItem* UrlList,List PageRankList){
    char *str = NULL;
    struct ListNode *curr = NULL;
    int idx;
    while(!QueueIsEmpty(lineContents)){
        str = QueueDequeue(lineContents);
        //find if str is already in URLlist
        if((idx = isStringInListArray(str,UrlList)) != -1){
            // go through pagerank find match to str ulr name 
            curr = PageRankList->first;
            
            
            while (curr != NULL) {
                if(strcmp(curr->str,str)==0){
                    // ListAppend(UrlList,curr->data,curr->str);
                    UrlList[idx].URL = malloc(sizeof(char)*(strlen(str)+1));
                    UrlList[idx].numMatches++;
                    UrlList[idx].pageRank = curr->data;
                    break;
                }
                curr = curr->next;
            
            }

        }
        free(str);
    }
}
void ArrayListShow(struct searchItem* UrlList){
    for(int i=0; UrlList[i].URL != NULL;i++){
        printf("Num Matches: %d, PageRank:%lf, URL %s",UrlList[i].numMatches,UrlList[i].pageRank,UrlList[i].URL);

    }
}
SearchItem* findMatchedUrls(const char* filename,char ** queryWords){
    FILE *fp;
    fp = fopen(filename, "r");
    if(fp == NULL){
        fprintf(stderr,"File pointer is NULL.\n");
        exit(EXIT_FAILURE);
    }
    size_t SIZE = 0;
    char *line = NULL;



    //char URL[MAX_URL_LENGTH];
    List pageRankList = getList("pagerankList.txt");
    SearchItem* UrlList = malloc(sizeof(SearchItem)*pageRankList->size);

    while(getline(&line,&SIZE,fp) != EOF){
        Queue lineContents = QueueNew();
        qUrls(line,lineContents);
        char * searchWord = QueueDequeue(lineContents);
        if(issearchWordinListArray(queryWords,searchWord) == true){
            //adds line contents i.e. URLs to UrlList if URL not already in List
            AddLineContents(lineContents,UrlList,pageRankList);
        }
        
        free(searchWord);
        QueueFree(lineContents);
    }
    free(line);
    fclose(fp);
    ListFree(pageRankList);
    return UrlList;
}

void ArrayFree(SearchItem * List){
    for(int i=0;List[i].URL != NULL;i++){
        free(List[i].URL);
        
    }
    free(List);
}
