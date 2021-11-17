// COMP2521 Assignment 2

// Written by: Jake Edwards
// Date: 16/11/2021

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
#include "IntList.h"

int main(int argc, char *argv[]) {
    for(int i =0; i <= argc ;i++){
        printf("%d : %s\n",i,argv[i]);
    }
    char *endPtr = NULL;
    double dampFactor, diffPR, maxIterations;
    //note last argument is always null
    if(argc < 4){
        fprintf(stderr,"Invalid No. Arguments %d, argv[1] %s\n",argc,argv[1]);
        exit(EXIT_FAILURE);
    }

    dampFactor = strtod(argv[1],&endPtr);
    diffPR = strtod(argv[2],&endPtr);
    maxIterations = strtod(argv[3],&endPtr);

    //skip line and EOF are dummy strings
    Queue collection_q = get_urls("collection.txt","skip line","EOF");

    //make graph size of collection q the queue index is the vertex number
    Graph urlGraph = GraphNew(QueueSize(collection_q));

    //populate vertex names
    GraphAddVertexNames(urlGraph,collection_q);

    //Add all Edges i.e. read	<url>.txt	file,	and	update	graph	by	adding	a	node	and	
    //outgoing	links	
    GraphPopulateEdges(urlGraph);

    //calculate PageRank for each url
    calculatePageRank(urlGraph,dampFactor,diffPR,maxIterations);

    //need to create a function which adds structs to a list in order of page rank
    IntList pageRankList = orderListURLS(urlGraph);

    //need to make a function that outputs this list to a pagerankList.txt file
    outputRankedURLS(urlGraph,pageRankList);
    printf("wtf\n");
    IntListShow(pageRankList);
    //navigate through queue in a while loop until reach end of collection queue
    IntListFree(pageRankList);
    GraphFree(urlGraph);
}

