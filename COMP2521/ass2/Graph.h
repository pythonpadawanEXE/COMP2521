// Interface to the Undirected Weighted Graph ADT
// - Vertices are identified by integers between 0 and nV - 1,
//   where nV is the number of vertices in the graph
// - Weights are doubles and must be positive
// - Self-loops are not allowed
//Taken from Lab week8

// !!! DO NOT MODIFY THIS FILE !!!

#ifndef GRAPH_H
#define GRAPH_H
#define MAX_URL_LEN 100

#include <stdbool.h>

#include "Queue.h"
#include "IntList.h"

typedef struct graph *Graph;

typedef int Vertex;

// edges are pairs of vertices (end-points)
typedef struct Edge {
    Vertex v;
    Vertex w;
    double weight;
} Edge;

/**
 * Creates a new instance of a graph
 */
Graph  GraphNew(int nV);

/**
 * Frees all memory associated with the given graph
 */
void   GraphFree(Graph g);

/**
 * Returns the number of vertices in the graph
 */
int    GraphNumVertices(Graph g);

/**
 * Inserts  an  edge into a graph. Does nothing if there is already an
 * edge between `e.v` and `e.w`. Returns true if successful, and false
 * if there was already an edge.
  */
bool   GraphInsertEdge(Graph g, Edge e);

/**
 * Removes an edge from a graph. Returns true if successful, and false
 * if the edge did not exist.
 */
bool   GraphRemoveEdge(Graph g, Vertex v, Vertex w);

/**
 * Returns the weight of the edge between `v` and `w` if it exists, or
 * 0.0 otherwise
 */
double GraphIsAdjacent(Graph g, Vertex v, Vertex w);

/**
 * Returns true if the graph contains a cycle, and false otherwise
 */
bool   GraphHasCycle(Graph g);

/**
 * Returns a minimum spanning tree of the given graph. The given graph
 * should not be modified. Returns NULL if the graph has no minimum 
 * spanning tree.
 */
Graph  GraphMST(Graph g);

/**
 * Displays information about the graph
 */
void   GraphShow(Graph g);

//ASS2 Functions
void calculatePageRank(Graph urlGraph,double dampFactor,double diffPR,int maxIterations);

void GraphPopulateEdges(Graph g);

void GraphAddVertexNames(Graph g,Queue q);

int GetURLNameIdx(Graph g,char * URL);

void GraphAddVertexNames(Graph g,Queue q);

IntList orderListURLS(Graph g);

void outputRankedURLS(Graph g,IntList l);

int findIndexFromPageRank(Graph g,double PageRank);

int ReturnOutDegree(Graph g,int idx);

char * ReturnURL(Graph g,int idx);

void outputRankedURLS(Graph g,IntList l);

void GraphGridShow(Graph g);

void GraphWeights(Graph g);



#endif
