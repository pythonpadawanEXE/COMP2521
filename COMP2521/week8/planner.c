// Algorithms to design electrical grids

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "Place.h"
#include "PQ.h"
typedef struct graph *Grid;
typedef Graph Grid;
struct graph {
    int nV;         // #vertices
    int nE;         // #edges
    double **edges; // adjacency matrix storing positive weights
                    // 0 if nodes not adjacent
};
////////////////////////////////////////////////////////////////////////
// Your task

/**
 * Designs  a minimal cost electrical grid that will deliver electricity
 * from a power plant to all the given cities. Power lines must be built
 * between cities or between a city and a power plant.  Cost is directly
 * proportional to the total length of power lines used.
 * Assumes  that  numCities  is at least 1 (numCities is the size of the
 * cities array).
 * Stores the power lines that need to be built in the given  powerLines
 * array, and returns the number of power lines stored. Assumes that the
 * powerLines array is large enough to store all required power lines.
 */

Edge ReturnEdgePlanner(Vertex v,Vertex w,double weight){
    Edge ret = {.v = v,.w = w,.weight = weight};
    return ret;
}
// Edge FindEdge(PQ pq,double *used_vertices){
//     //navigate PQ and check if vertices are not being used in used_vertices
//     //used vertices will have a value equal to 1 unused equal to 0
   
//     while (PQIsEmpty(pq) == false){// && invalidedge == true){
//         Edge e = PQExtract(pq);
//         //check if Edge vertices are used
//         if(e.weight > 0 && e.v != e.w){
//             if(used_vertices[e.v] == 1 && used_vertices[e.w] == 0 ){
//                 used_vertices[e.w] = 1;
//                 return e;
//             }
//             else if(used_vertices[e.v] == 0 && used_vertices[e.w] == 1 ){
//                 used_vertices[e.v] = 1;
//                 return e;
//             }
//             else if(used_vertices[e.v] == 0 && used_vertices[e.w] == 0 ){
//                 used_vertices[e.v] = 1;
//                 used_vertices[e.w] = 1;
//                 return e;
//             }
//         }
//     }
    
//     return ReturnEdge(-1,-1,-1);
    
// }



// void PopulateUnusedE(PQ pq,Graph g,int numCities){
//     int nV = numCities + 1;
//     Edge tmp;
//     for(int i = 0;i < nV; i++){
//         for(int j =0; j < nV ;j++){
//             tmp = ReturnEdge(i,j,g->edges[i][j]);
//             PQInsert(pq,tmp);
//         }
//     }
// }

// Graph GraphMSTPlanner(Graph g,int numCities) {
//     // TODO: Complete this function

//     /*
//     PrimMST(G):
//     |  Input  graph G with n nodes
//     |  Output a minimum spanning tree of G
//     |
//     |  MST=empty graph
//     |  usedV={0}
//     |  unusedE=edges(g)
//     |  while |usedV| < n do
//     |  |  find e=(s,t,w) in unusedE such that {
//     |  |     s in usedV and t not in usedV 
//     |  |       and w is min weight of all such edges
//     |  |  }
//     |  |  MST = MST U {e}
//     |  |  usedV = usedV U {t}
//     |  |  unusedE = unusedE \ {e}
//     |  end while
//     |  return MST
//     */

//     //use priority queue
//     int nV = numCities+1;
//     Graph MST = GraphNew(nV); 
//     double *used_vertices = calloc(nV , sizeof(double));
//     if (used_vertices == NULL) {
//             fprintf(stderr, "error: out of memory\n");
//             exit(EXIT_FAILURE);
//     }
//     PQ pq =  PQNew(); //unused E
//     PopulateUnusedE(pq,g);
//     Edge E;
//     int size_usedV = 0;
//     while(PQIsEmpty(pq) == false){
//         E = FindEdge(pq,used_vertices);
//         if( E.weight != -1){
//             GraphInsertEdge(MST,E);
//             size_usedV++; 
//         }
//     }

//     free(used_vertices);
//     PQFree(pq);
//     if(size_usedV != nV -1){
//         return NULL;
//     }
//     return MST;
    
// }
Place ReturnPlace(int j,int nV,Place cities[],Place powerPlant){
    if(j == nV-1){
        return powerPlant;
    }
    return cities[j];
}

PowerLine ReturnPowerLine(Place p1,Place p2){
    PowerLine ret = {.p1=p1 ,.p2=p2};
    return ret;
}

int CalculateWeight(int i,int j,Place cities[],Place powerPlant,int numCities){
    //int nV = numCities + 1;
    int x1;
    int x2;
    int y1;
    int y2;

    if(i < numCities && j == numCities){
        x1 = cities[i].x;
        y1 = cities[i].y;
        x2 = powerPlant.x;
        y2 = powerPlant.y;
    }
    else if (i < numCities && j < numCities){
        x1 = cities[i].x;
        y1 = cities[i].y;
        x2 = cities[j].x;
        y2 = cities[j].y;
    }
    return sqrt(pow((x2-x1),2) + pow((y2-y1),2));
}
int planGrid1(Place cities[], int numCities, Place powerPlant,
              PowerLine powerLines[]) {
    
    //place is a vertex
    //powerline is an edge
    //make graph with numCities and place power plant with everything being an edge
    

    //define total vertices
    int nV = numCities + 1;
    
    //make graph with everything connected then find  MST from this
    Graph AllEdges = GraphNew(nV); 
    Edge E;
    //make priority queue and add all the possible cities connections then at line 186-188 add the power plant connection to those city connections

    PQ pq = PQNew();
    //vertex in Edge will be index in cities last vertex is powerplant
    for(int i = 0;i < numCities ; i++){
        for(int j = 0; j< numCities;j++){
            //prevent duplicates in two ways either being self loops or same relationship in other direction as undirected
            if(i != j && i < j){
                E = ReturnEdgePlanner(i,j,CalculateWeight(i,j,cities,powerPlant,numCities));
                GraphInsertEdge(AllEdges,E);
                PQInsert(pq,E);
            }
        }
        E = ReturnEdgePlanner(i,numCities,CalculateWeight(i,numCities,cities,powerPlant,numCities));
        GraphInsertEdge(AllEdges,E);
        PQInsert(pq,E);

    }
    
    int count = 0;
    //Get the MST from graph
    Graph MST = GraphMST(AllEdges);
    //HACK THE ADT thru typedef

    //go through all the edges in the edges array and add to the powerline make sure to only add edge once by doing i>j
    for(int i = 0; i < nV ; i++){
        for(int j = 0; j < nV;j++){
            if (MST->edges[i][j] > 0 && i < j){
                powerLines[count] = ReturnPowerLine(ReturnPlace(i,nV,cities,powerPlant),ReturnPlace(j,nV,cities,powerPlant));
                count++;
            }
        }   
    }
    int n_lines = MST->nE;
    
    PQFree(pq);
    GraphFree(MST);
    GraphFree(AllEdges);
    return n_lines;
}

////////////////////////////////////////////////////////////////////////
// Optional task

/**
 * Designs  a minimal cost electrical grid that will deliver electricity
 * to all the given cities.  Power lines must be built between cities or
 * between a city and a power plant.  Cost is directly  proportional  to
 * the  total  length of power lines used.  Assume that each power plant
 * generates enough electricity to supply all cities, so not  all  power
 * plants need to be used.
 * Assumes  that  numCities and numPowerPlants are at least 1 (numCities
 * and numPowerPlants are the sizes of the cities and powerPlants arrays
 * respectively).
 * Stores the power lines that need to be built in the given  powerLines
 * array, and returns the number of power lines stored. Assumes that the
 * powerLines array is large enough to store all required power lines.
 */
int planGrid2(Place cities[], int numCities,
              Place powerPlants[], int numPowerPlants,
              PowerLine powerLines[]) {
    // TODO: Complete this function
    return 0;
}
