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
#include "IntList.h"


struct graph {
    int nV;         // #vertices
    int nE;         // #edges
    double **edges; // adjacency matrix storing positive weights
    char **verticeNames;
    double *oldpageRanks;
    double *pageRanks;
    double *outDegree; //for each URL how many links that URL has where the index of numLinks and vertice_names match
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
//adds vertex names to vertice_names which can be used to craw url.txt files later
void GraphAddVertexNames(Graph g,Queue q){
    char *q_head_str = NULL;
    //goes through queue and copy queue items to graph vertice names
    for(int i = 0;i<g->nV;i++){
        g->verticeNames[i] = malloc(MAX_URL_LEN * sizeof(char));
        q_head_str = QueueDequeue(q);

        strcpy(g->verticeNames[i],q_head_str);
        
        free(q_head_str);

    }
    QueueFree(q);
}

//populates the graph edges by crawling the urls in the vertice_names char array once that has been made
void GraphPopulateEdges(Graph g){
    Queue outgoing_links;
    char *filename = malloc(sizeof(char )* (MAX_URL_LEN + strlen(".txt")));
    char suffix[] = ".txt";
    char *q_head_str = NULL;
    // int num_links = -1;
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
        outgoing_links = get_urls(filename,"#start Section-1\n","#end Section-1\n");
        
        // num_links= QueueSize(outgoing_links);

        //O_u is num_links for each URL
        
        //v is the from position i.e. outgoing
        e.v = i;
        while(!QueueIsEmpty(outgoing_links)){
            //dequeue to get string
            q_head_str = QueueDequeue(outgoing_links);
            if( strcmp(q_head_str,g->verticeNames[i]) != 0 ){
                g->outDegree[i]++;
                //find index of link name (separate function)
                idx = GetURLNameIdx(g,q_head_str);
                //URL name not found .. what do? continue? exit? should this never happen?
                if(idx == -1){
                fprintf(stderr, "URL doesn't seem to have been found\n");
                    exit(EXIT_FAILURE);
                }
                //e.w is the to position or the incoming
                e.w = idx;
                g->inDegree[idx]++;
                GraphInsertEdge(g,e);
                // if(e.w != e.v){
                //     GraphInsertEdge(g,e);
                // }
            }
            free(q_head_str);
        }
        QueueFree(outgoing_links);
    }
    free(filename);

}
/*
W_out(v,u)
O_u is the degree of outlinks from vertex u
the denominator is the sum of the outlinks from each O_p
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
    if (sumP == 0){
        printf("?\n");
    }
    double ret = oU/sumP;
    return ret;
}
/*
W_in(v,u)
i_U is the degree of inlinks to vertex u
the denominator is the sum of the inlinks to each O_p
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
    if (sumP == 0){
        printf("?\n");
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
                    //will i>j get rid of parallel edges?
                    if(urlGraph->edges[j][i] > 0){
                        printf("iteration:%d i:%d j:%d sum %lf rank %lf wIN %lf wOut %lf\n",iteration,i,j,sum,urlGraph->pageRanks[i],wIn(urlGraph,j,i),wOut(urlGraph,j,i));
                        sum = sum + urlGraph->pageRanks[i] * wIn(urlGraph,j,i) * wOut(urlGraph,j,i);
                        
                        
                    }
                       
                }
                urlGraph->oldpageRanks[i] = urlGraph->pageRanks[i];
                urlGraph->pageRanks[i] = (1-dampFactor)/N + dampFactor * sum;
            }
        }
        // for(int i = 0; i < N;i++){
        //     printf("%lf\n",urlGraph->pageRanks[i]);
        //     if(urlGraph->pageRanks[i] > 1){

        //     }
        // }
        
        diff = getDiff(urlGraph);
        iteration++;
    }


}
//make ordered list of pageRanks
IntList orderListURLS(Graph g){
    IntList l = IntListNew();
    for(int i = 0;i < g->nV;i++){
        IntListInsertInOrder(l,g->pageRanks[i]);
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
void outputRankedURLS(Graph g,IntList l){
	int idx;
	char *URL;
	int outDegree;
	//make file to write to
	FILE *fptr;
	fptr = fopen("pagerankList.txt","w");
	for (struct IntListNode *curr = l->first;
	curr != NULL; curr = curr->next) {
		idx = findIndexFromPageRank(g,curr->data);
		outDegree = ReturnOutDegree(g,idx);
		URL = ReturnURL(g,idx);
		fprintf(fptr,"%s, %d, %.7lf\n",URL,outDegree,curr->data);

	}
	fclose(fptr);
}

////////////////////////////////////////////////////////////////////////
// Your task (week8 Lab)

Edge ReturnEdge(Vertex v,Vertex w,double weight){
    Edge ret = {.v = v,.w = w,.weight = weight};
    return ret;
}

Edge FindEdge(Graph MST,PQ pq,double *used_vertices){
    //navigate PQ and check if vertices are not being used in used_vertices
    //used vertices will have a value equal to 1 unused equal to 0
   //use tmp priority queue to store edges
   
    PQ tmp = PQNew();
  
    Edge e;
    //to create first edge
    // if(MST->nE == 0){
    //     e = PQExtract(pq);
    //     return e;
    // }
    //e is not guaranteed to be adjacent FIX

    
    bool found_edge = false;
    while(found_edge == false){
        if(PQIsEmpty(pq) == true){
            e = ReturnEdge(-1,-1,-1);
            break;
        }
        e = PQExtract(pq);
        //!= serves as XOR
        
        if(e.weight > 0 && e.v != e.w && ((used_vertices[e.v] == 0) != (used_vertices[e.w] == 0) )){
            used_vertices[e.w] = 1;
            used_vertices[e.v] = 1;
            found_edge = true;   
        }
        else{
            PQInsert(tmp,e);
        }
    }
    //put unused edges back into tmp
    Edge tmp_e;
    while(!PQIsEmpty(tmp)){
                tmp_e = PQExtract(tmp);
                PQInsert(pq,tmp_e);
    }
    PQFree(tmp);
    return e;
    
}



void PopulateUnusedE(PQ pq,Graph g){
    Edge tmp;
    for(int i = 0;i < g->nV; i++){
        for(int j =0; j <g->nV ;j++){
            if(i > j && i != j && g->edges[i][j] > 0){
                tmp = ReturnEdge(i,j,g->edges[i][j]);
                PQInsert(pq,tmp);
            }
        }
    }
}

Graph GraphMST(Graph g) {
    // TODO: Complete this function

    /*
    PrimMST(G):
    |  Input  graph G with n nodes
    |  Output a minimum spanning tree of G
    |
    |  MST=empty graph
    |  usedV={0}
    |  unusedE=edges(g)
    |  while |usedV| < n do
    |  |  find e=(s,t,w) in unusedE such that {
    |  |     s in usedV and t not in usedV 
    |  |       and w is min weight of all such edges
    |  |  }
    |  |  MST = MST U {e}
    |  |  usedV = usedV U {t}
    |  |  unusedE = unusedE \ {e}
    |  end while
    |  return MST
    */

    //use priority queue
    int nV = g->nV;
    Graph MST = GraphNew(nV); 
    double *used_vertices = calloc(nV , sizeof(double));
    if (used_vertices == NULL) {
            fprintf(stderr, "error: out of memory\n");
            exit(EXIT_FAILURE);
    }
    PQ pq =  PQNew(); //unused E
    PopulateUnusedE(pq,g);
    //PQShow(pq);
    // while(PQIsEmpty(pq) == false){
    //     Edge tmp = PQExtract(pq);
    //     printf("%d %d %lf\n",tmp.w,tmp.v,tmp.weight);
    // }
    // return NULL;
    Edge E;
    int size_usedV = 0;
    //initlaise used_vertices to allow for XOR and start at first position
    used_vertices[0] = 1;
    while(size_usedV < nV){
        E = FindEdge(g,pq,used_vertices);
        //printf("E:%d %d %lf\n",E.w,E.v,E.weight);
        if( E.weight != -1){
            GraphInsertEdge(MST,E);
            size_usedV++; 
        }
        else{
            break;
        }
    }
    //GraphShow(MST);
    free(used_vertices);
    PQFree(pq);
    //printf("%d %d\n",size_usedV,nV-1);
    if(MST->nV-1 != MST->nE){
        return NULL;
    }
    return MST;
    
}




////////////////////////////////////////////////////////////////////////

static int validVertex(Graph g, Vertex v) {
    return v >= 0 && v < g->nV;
}

