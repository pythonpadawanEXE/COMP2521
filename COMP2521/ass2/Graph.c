// Implementation of the Undirected Weighted Graph ADT
// Uses an adjacency matrix
//Taken from Lab week8

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Graph.h"
#include "PQ.h"
#include "Queue.h"
#include "readData.h"
#include "List.h"


struct graph {
    int nV;         // #vertices
    int nE;         // #edges
    double **edges; // adjacency matrix storing positive weights
    char **verticeNames;
    double *oldpageRanks;
    double *pageRanks;
    double *outDegree; //for each URL how many links that URL has where the index of numLinks and verticeNames match
    double *inDegree; //for each URL how many links are pointing to it i.e. incoming link counter
                    // 0 if nodes not adjacent
};

static bool doHasCycle(Graph g, Vertex v, Vertex prev, bool *visited);
static int  validVertex(Graph g, Vertex v);

////////////////////////////////////////////////////////////////////////

Graph GraphNew(int nV) {
    assert(nV > 0);

    Graph g = malloc(sizeof(*g));
    if (g == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    g->nV = nV;
    g->nE = 0;
    //ADD vertice names
    g->verticeNames = malloc(nV * sizeof(char *));
    assert(g->verticeNames != NULL);

    //add number outgoing links for each vertex
    g->outDegree = calloc(nV,sizeof(double));
    assert(g->outDegree != NULL);

    //add number incoming links for each vertex
    g->inDegree = calloc(nV,sizeof(double));
    assert(g->inDegree != NULL);

    //add page Ranks
    g->pageRanks = calloc(nV,sizeof(double));
    assert(g->pageRanks != NULL);

    g->oldpageRanks = calloc(nV,sizeof(double));
    assert(g->oldpageRanks != NULL);

    g->edges = malloc(nV * sizeof(double *));
    if (g->edges == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < nV; i++) {
        //initalise to NULL to help find segfaults
        g->verticeNames[i] = NULL;
        g->edges[i] = calloc(nV, sizeof(double));
        if (g->edges[i] == NULL) {
            fprintf(stderr, "error: out of memory\n");
            exit(EXIT_FAILURE);
        }
    }

    return g;
}

void GraphFree(Graph g) {
    for (int i = 0; i < g->nV; i++) {
        free(g->edges[i]);
        //free vertice names
        free(g->verticeNames[i]);
    }
    free(g->inDegree);
    free(g->pageRanks);
    free(g->oldpageRanks);
    free(g->outDegree);
    free(g->edges);
    free(g->verticeNames);
    free(g);
}

////////////////////////////////////////////////////////////////////////

int GraphNumVertices(Graph g) {
    return g->nV;
}

bool GraphInsertEdge(Graph g, Edge e) {
    assert(validVertex(g, e.v));
    assert(validVertex(g, e.w));
    assert(e.v != e.w);
    assert(e.weight > 0.0);

    if (g->edges[e.v][e.w] == 0.0) {
        g->edges[e.v][e.w] = e.weight;
        //g->edges[e.w][e.v] = e.weight;
        g->nE++;
        return true;
    } else {
        return false;
    }
}

bool GraphRemoveEdge(Graph g, Vertex v, Vertex w) {
    assert(validVertex(g, v));
    assert(validVertex(g, w));

    if (g->edges[v][w] != 0.0) {   // edge e in graph
        g->edges[v][w] = 0.0;
        g->edges[w][v] = 0.0;
        g->nE--;
        return true;
    } else {
        return false;
    }
}

double GraphIsAdjacent(Graph g, Vertex v, Vertex w) {
    assert(validVertex(g, v));
    assert(validVertex(g, w));
    
    return g->edges[v][w];
}

void GraphShow(Graph g) {
    printf("Number of vertices: %d\n", g->nV);
    printf("Number of edges: %d\n", g->nE);
    for (int v = 0; v < g->nV; v++) {
        for (int w = v + 1; w < g->nV; w++) {
            if (g->edges[v][w] != 0.0) {
                printf("Edge %d - %d: %lf\n", v, w, g->edges[v][w]);
            }
        }
    }
}

void GraphGridShow(Graph g){
    for (int v = 0; v < g->nV; v++) {
        for (int w = 0; w < g->nV; w++) {
            printf("%lf ",g->edges[v][w]);
        }
        printf("\n");
    }
}
void GraphWeights(Graph g){
    for (int v = 0; v < g->nV; v++) {
        printf("InDegree: %lf, OutDegree %lf, Name %s\n",g->inDegree[v],g->outDegree[v],g->verticeNames[v]);
    }
}

bool GraphHasCycle(Graph g) {
    bool *visited = calloc(g->nV, sizeof(bool));
    assert(visited != NULL); // lazy error checking
    
    for (int v = 0; v < g->nV; v++) {
        if (!visited[v] && doHasCycle(g, v, v, visited)) {
            free(visited);
            return true;
        }
    }

    free(visited);
    return false;
}

static bool doHasCycle(Graph g, Vertex v, Vertex prev, bool *visited) {
    visited[v] = true;
    for (int w = 0; w < g->nV; w++) {
        if (g->edges[v][w] != 0.0) {
            if (!visited[w]) {
                if (doHasCycle(g, w, v, visited)) {
                    return true;
                }
            } else if (w != prev) {
                return true;
            }
        }
    }
    return false;
}
////////////////////////////////////////////////////////////////////////
// Ass 2 new functions
//find the index in the graph rep for the given URL string
int GetURLNameIdx(Graph g,char * URL){
    for(int i = 0;i < g->nV;i++){
        if(strcmp(g->verticeNames[i],URL) == 0){
            return i;
        }
    }
    return -1;
}
//adds vertex names to verticeNames which can be used to craw url.txt files later
void GraphAddVertexNames(Graph g,Queue q){
    char *qHeadStr = NULL;
    //goes through queue and copy queue items to graph vertice names
    for(int i = 0;i<g->nV;i++){
        g->verticeNames[i] = malloc(MaxUrlLen * sizeof(char));
        qHeadStr = QueueDequeue(q);

        strcpy(g->verticeNames[i],qHeadStr);
        
        free(qHeadStr);

    }
    QueueFree(q);
}

//populates the graph edges by crawling the urls in the verticeNames char array once that has been made
void GraphPopulateEdges(Graph g){
    Queue outgoingLinks;
    char *filename = malloc(sizeof(char )* (MaxUrlLen + strlen(".txt")));
    char suffix[] = ".txt";
    char *qHeadStr = NULL;
    
    int idx;
    Edge e;
    e.v = -1;
    e.w = -1;
    e.weight = 1;
    for(int i = 0;i < g->nV;i++){
        //copyfile contents to filename
        strcpy(filename,g->verticeNames[i]);
        //append .txt to filename
        strcat(filename,suffix);
        //get list of out going links from filename
        outgoingLinks = getUrls(filename,"#start Section-1\n","#end Section-1\n");
 
        
        //v is the from position i.e. outgoing
        e.v = i;
        while(!QueueIsEmpty(outgoingLinks)){
            //dequeue to get string
            qHeadStr = QueueDequeue(outgoingLinks);
            if( strcmp(qHeadStr,g->verticeNames[i]) != 0 ){
                g->outDegree[i]++;
                //find index of link name (separate function)
                idx = GetURLNameIdx(g,qHeadStr);
                //URL name not found .. what do? continue? exit? should this never happen?
                if(idx == -1){
                fprintf(stderr, "URL doesn't seem to have been found\n");
                    exit(EXIT_FAILURE);
                }
                //e.w is the to position or the incoming
                e.w = idx;
                g->inDegree[idx]++;
                GraphInsertEdge(g,e);
            }
            free(qHeadStr);
        }
        QueueFree(outgoingLinks);
    }
    free(filename);

}
/*
Wout(v,u)
oU is the degree of outlinks from vertex u
the denominator is the sum of the outlinks from each oP
where p is in (an element of) v which means
from vertex v to vertex p there is a link i.e. edges[v][p] > 0
*/
double wOut(Graph g,int v,int u){
    double oU= g->outDegree[u];
    double sumP = 0;
    double half = 0.5;
    for(int p = 0;p < g->nV;p++){
        if(g->edges[v][p] > 0){
            if(g->outDegree[p] > 0){
                sumP = sumP + g->outDegree[p];
            }
            else{
                sumP = sumP + half;
            }
        }
    }
    double ret = oU/sumP;
    return ret;
   
}
/*
wIn(v,u)
iU is the degree of inlinks to vertex u
the denominator is the sum of the inlinks to each iP
where p is in (an element of) v which means
from vertex p to vertex v there is a link i.e. edges[p][v] > 0
*/
double wIn(Graph g,int v,int u){
    double iU = g->inDegree[u];
    double sumP = 0;
    for(int p = 0;p < g->nV;p++){
        if(g->edges[v][p] > 0){
            sumP = sumP + g->inDegree[p];
        }
    }
    double ret = iU/sumP;
    return ret;
}

static double getDiff(Graph g){
    double diff = 0;
    for(int i =0;i < g->nV;i++){
        diff = diff + fabs(g->pageRanks[i] - g->oldpageRanks[i]);
    }
    return diff;
}
void printWinWout(Graph g){
    for(int j = 0;j < g->nV;j++){
        for(int i =0;i < g->nV;i++){
            if(g->edges[j][i] > 0){
                printf("Win[%d][%d]: %.7lf Wout[%d][%d]:%.7lf\n",j,i,wIn(g,j,i),j,i,wOut(g,j,i));
            }
        }
    }
}
//calculates the page rank for each url/vertex in the graph
void calculatePageRank(Graph urlGraph,double dampFactor,double diffPR,int maxIterations){
    int iteration = 0;
    double diff = diffPR;
    double N = urlGraph->nV;
    double half = 0.5;
    double sum = 0;
    //fix outdegree
    for(int i = 0;i< N;i++){
        if(urlGraph->outDegree[i]==0){
            urlGraph->outDegree[i] = half;
        }
    }
    
    
    while(iteration < maxIterations && diff >= diffPR){
        
        if(iteration == 0){
            for(int i = 0;i < N;i++){
                urlGraph->oldpageRanks[i] = 0;
                urlGraph->pageRanks[i]  = 1/N;
                
            }
        }
        else{
            
            for(int i = 0;i < N;i++){
                sum = 0;
                
                //j is an element of i as edges[i][j] > 0
                for(int j = 0; j < N;j++){
                    
                    if(urlGraph->edges[j][i] > 0){
                        
                        sum = sum + urlGraph->pageRanks[j] * wIn(urlGraph,j,i) * wOut(urlGraph,j,i);
                        
                        
                    }
                       
                }
                urlGraph->oldpageRanks[i] = urlGraph->pageRanks[i];
                urlGraph->pageRanks[i] = (1-dampFactor)/N + dampFactor * sum;
            }
        }
        
        
        diff = getDiff(urlGraph);
        iteration++;
    }

}
int comparePageRank(int idx1,int idx2,Graph g){
    //if(g->pageRanks[idx2] != g->pageRanks[idx1]){
        return g->pageRanks[idx2] > g->pageRanks[idx1];
    // }
    // return strcmp(g->verticeNames[idx2],g->verticeNames[idx1]) > 0;
}
//sort by a then by b
void insertionSort(Graph g,double a[],char *b[], int lo, int hi)
{
   int i, j,ncomps = 0;
   double val;
   for (i = lo+1; i <= hi; i++) {
      val = a[i];
      for (j = i; j > lo; j--) {
        
        ncomps++;
        //stops scan when position found
         if (comparePageRank(i,j-1,g)) break;
         a[j] = a[j-1];
      }
      a[j] = val;
   }
   
}

//make ordered list of pageRanks
List orderListURLS(Graph g){
    List l = ListNew();
    // insertionSort(g,g->pageRanks,g->verticeNames,0,g->nV);
    for(int i = 0;i < g->nV;i++){
        ListInsertRevInOrder(l,g->pageRanks[i],g->verticeNames[i]);
        // ListPrepend(l,g->pageRanks[i],g->verticeNames[i]);
    }
    return l;
}
int findIndexFromPageRank(Graph g,double PageRank){
    for(int i = 0;i < g->nV ; i++){
        if(PageRank == g->pageRanks[i]){
            return i;
        }
    }
    return -1;
}

int ReturnOutDegree(Graph g,int idx){
    return g->outDegree[idx];
}

char * ReturnURL(Graph g,int idx){
    return g->verticeNames[idx];
}
void outputRankedURLS(Graph g,List l){
	int idx;
	char *URL;
	int outDegree;
	//make file to write to
	FILE *fptr;
	fptr = fopen("pagerankList.txt","w");
	for (struct ListNode *curr = l->first;
	curr != NULL; curr = curr->next) {
		idx = findIndexFromPageRank(g,curr->data);
		outDegree = ReturnOutDegree(g,idx);
		URL = ReturnURL(g,idx);
		fprintf(fptr,"%s, %d, %.7lf\n",URL,outDegree,curr->data);

	}
	fclose(fptr);
}


static int validVertex(Graph g, Vertex v) {
    return v >= 0 && v < g->nV;
}

