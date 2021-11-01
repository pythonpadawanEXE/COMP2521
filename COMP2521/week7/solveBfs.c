// BFS maze solver

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Cell.h"
#include "helpers.h"
#include "Maze.h"
#include "Queue.h"

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
Cell Start = MazeGetStart(m);
Cell V;
MazeVisit(Start);
Queue Q = QueueNew(); //rmb to free
QueueEnqueue(Q,Start);

while(found == false && QueueSize(Q) > 0){
    V = QueueDequeue(Q);
    if(MazeVisit(V) == true){
        found = true;
    }
    else{
        for(int row = 0;row < MazeWidth(m);row++){
            for(int col = 0;col < MazeHeight(m); col++){
                Cell x = {row,col};
                if(visited = None && MazeIsWall(m,C) == false){
                    MazeVisit(V);
                    //when do we mark part of  the path
                    enqueue vertex into Q
                }
                
            }
        }
         
            
            
    }

}


    return found;
}

