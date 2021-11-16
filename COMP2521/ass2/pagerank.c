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

int main(int argc, char *argv[]) {
    int dampFactor, diffPR, maxIterations;
    dampFactor = atoi(argv[1]);
    diffPR = atoi(argv[2]);
    maxIterations = atoi(argv[3]);

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


    //navigate through queue in a while loop until reach end of collection queue
    GraphFree(urlGraph);
}

