// Implementation of the Undirected Weighted Graph ADT
// Uses an adjacency matrix

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "PQ.h"

struct graph {
    int nV;         // #vertices
    int nE;         // #edges
    double **edges; // adjacency matrix storing positive weights
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

    g->edges = malloc(nV * sizeof(double *));
    if (g->edges == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < nV; i++) {
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
    }
    free(g->edges);
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
        g->edges[e.w][e.v] = e.weight;
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
// Your task
Edge ReturnEdge(Vertex v,Vertex w,double weight){
    Edge ret = {.v = v,.w = w,.weight = weight};
    return ret;
}
Edge FindEdge(Graph g,PQ pq,double *used_vertices){
    //navigate PQ and check if vertices are not being used in used_vertices
    //used vertices will have a value equal to 1 unused equal to 0
   
   // && invalidedge == true){
    Edge e = PQExtract(pq);
    //check if Edge vertices are used
    if(e.weight == 313.000000){
        printf("test\n");
    }
    if(e.weight > 0 && e.v != e.w && GraphIsAdjacent(g, e.v,e.w)){
        if(used_vertices[e.v] == 1 && used_vertices[e.w] == 0 ){
            used_vertices[e.w] = 1;
            return e;
        }
        else if(used_vertices[e.v] == 0 && used_vertices[e.w] == 1 ){
            used_vertices[e.v] = 1;
            return e;
        }
        else if(used_vertices[e.v] == 0 && used_vertices[e.w] == 0 ){
            used_vertices[e.v] = 1;
            used_vertices[e.w] = 1;
            return e;
        }
    }
    
    
    return ReturnEdge(-1,-1,-1);
    
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
    PQShow(pq);
    Edge E;
    int size_usedV = 0;
    while(PQIsEmpty(pq) == false){
        E = FindEdge(g,pq,used_vertices);
        if( E.weight != -1){
            GraphInsertEdge(MST,E);
            size_usedV++; 
        }
    }
    GraphShow(MST);
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

