// BFS maze solver

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Cell.h"
#include "helpers.h"
#include "Maze.h"
#include "Queue.h"
typedef enum {
    NONE,
    VISITED,
    PATH,
} State;

bool isvalid(Maze m,Cell V){
    int Height = MazeHeight(m);
    int Width = MazeWidth(m);
    //if within bounds
    if(V.row < Height && V.row >= 0 && V.col < Width && V.col >= 0 && MazeIsWall(m,V) == false){
        return true;
    }
    else{
        return false;
    }

}
bool adjacent(Maze m,Cell V,Cell W){
    Cell Left = {V.row,V.col-1};
    Cell Right = {V.row,V.col+1};
    Cell Up = {V.row+1,V.col};
    Cell Down = {V.row-1,V.col};
    if(isvalid(m,W) == true && isvalid(m,V)){
        if(isvalid(m,Left) == true){
            if(Left.row == W.row && Left.col == W.col){
                return true;
            }
        }
        if(isvalid(m,Right) == true){
            if(Right.row == W.row && Right.col == W.col){
                return true;
            }
        }
        if(isvalid(m,Up) == true){
            if(Up.row == W.row && Up.col == W.col){
                return true;
            }
        }
        if(isvalid(m,Down) == true){
            if(Down.row == W.row && Down.col == W.col){
                return true;
            }
        }
    }
        return false;
}
Cell ReturnCell(int row,int col){
    Cell x ={.row = row,.col = col};
    return x;
}

bool solve(Maze m) {
    // TODO: Complete this function
    //       Feel free to add helper functions
    /*
|  Input  graph G, vertices src,dest
|
|  for all vertices v in G do
|     visited[v]=-1
|  end for
|  found=false
|  visited[src]=src
|  enqueue src into queue Q
|  while not found && Q is not empty do
|  |  dequeue v from Q
|  |  if v=dest then
|  |     found=true
|  |  else
|  |  |  for each (v,w) in edges(G) with visited[w]=-1 do
|  |  |     visited[w]=v
|  |  |     enqueue w into Q
|  |  |  end for
|  |  end if
|  end while
|  if found then
|     display path in dest..src order
|  end if
*/
bool found = false;
bool **boolmatrix = createBoolMatrix(MazeHeight(m), MazeWidth(m));
Cell **predecessormatrix = createCellMatrix(MazeHeight(m), MazeWidth(m));
Cell Start = MazeGetStart(m);
Cell V;
Cell W;

boolmatrix[Start.row][Start.col] = VISITED;
Queue Q = QueueNew(); //rmb to free
QueueEnqueue(Q,Start);

while(!QueueIsEmpty(Q)){
    V = QueueDequeue(Q);
    
    if(MazeVisit(m,V) == true){
        while(V.row != Start.row || V.col != Start.col){
            MazeMarkPath(m,V);
            V   = predecessormatrix[V.row][V.col];
        }
        MazeMarkPath(m,V);
        MazeMarkPath(m,Start); 
        found = true;
        return found;
    }
    else{
        for(int row = 0;row < MazeHeight(m);row++){
            for(int col = 0;col < MazeWidth(m); col++){
                
                W = ReturnCell(row,col);
                
                if(boolmatrix[W.row][W.col] == NONE && adjacent(m,V,W) == true){// && W.row != Start.row && W.col != Start.col){
                    
                    predecessormatrix[W.row][W.col] = V;
                    MazeVisit(m,W);
                    boolmatrix[W.row][W.col] = VISITED;
                    
                    
                    
                    QueueEnqueue(Q,W);
                    
                }
                
            }
        }
         
            
            
    }

}


// bool found = false;
// Cell v;
// bool **boolmatrix = createBoolMatrix(MazeHeight(m), MazeWidth(m));

// for (v = 0; v < nV; v++)
//       visited[v] = -1;

//    visited[src] = src;
//    queue Q = newQueue();
//    QueueEnqueue(Q, src);
//    while (!QueueIsEmpty(Q)) {
//       v = QueueDequeue(Q);
//       Vertex w;
//       for (w = 0; w < nV; w++)
// 	 if (adjacent(g, v, w) && visited[w] == -1) {
// 	    visited[w] = v;
// 	    if (w == dest)
// 	       return true;
// 	    else
// 	       QueueEnqueue(Q, w);
// 	 }
//    }
//    return false;
// }

    freeCellMatrix(predecessormatrix);
    freeBoolMatrix(boolmatrix);
    QueueFree(Q);
    return found;
}

