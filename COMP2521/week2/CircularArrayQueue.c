// Implementation of the Queue ADT using a circular array

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Queue.h"

#define DEFAULT_SIZE 16 // DO NOT change this line

// DO NOT modify this struct
struct queue {
	Item *items;
	int size;
	int capacity;
	int frontIndex;
};

/**
 * Creates a new empty queue
 */
Queue QueueNew(void) {
	Queue q = malloc(sizeof(*q));
	if (q == NULL) {
		fprintf(stderr, "couldn't allocate Queue");
		exit(EXIT_FAILURE);
	}

	q->items = malloc(DEFAULT_SIZE * sizeof(Item));
	if (q->items == NULL) {
		fprintf(stderr, "couldn't allocate Queue");
		exit(EXIT_FAILURE);
	}

	q->size = 0;
	q->capacity = DEFAULT_SIZE;
	q->frontIndex = 0;
	return q;
}

/**
 * Frees all resources associated with the given queue
 */
void QueueFree(Queue q) {
	free(q->items);
	free(q);
}

/**
 * Adds an item to the end of the queue
 */
void QueueEnqueue(Queue q, Item it) {
	if(q->size == q->capacity){
	    int old_last_index = q->capacity-1;
	    q->capacity = q->capacity + DEFAULT_SIZE;
	    
	    q->items = realloc(q->items,q->capacity * sizeof(Item));
	    //int overlap = ;
	    //move item after reallocation which are before the first_index
	    //in the circular array (when perceived as a regular array)
	    
	    
	    for(int index_to_move = 0; index_to_move < q->frontIndex;index_to_move++){
	        
	        old_last_index++;
	        //printf("Idx move:%d, Idx old_last:%d ,front:%d new val:%d\n",index_to_move,old_last_index,q->frontIndex,q->items[index_to_move]);
	        //move elements by stretching circle array move element in index_to_move
	        //from circular implementation to straightened implementation in new memory
	        //with respect to first index and the capacity index i.e. old_last_index
	        //which moves as more elements are moved
	        q->items[old_last_index] = q->items[index_to_move];
	    
	    }
	    
	    

	}

    q->items[(q->size+q->frontIndex)%q->capacity] = it;
    q->size++;
    

}

/**
 * Removes an item from the front of the queue and returns it
 * Assumes that the queue is not empty
 */
Item QueueDequeue(Queue q) {
	if(q->size == 0){
	    fprintf(stderr, "couldn't Dequeue list appears empty");
		exit(EXIT_FAILURE);
	}
	Item item = q->items[q->frontIndex];
    q->size--;
    q->items[q->frontIndex] = -1;
    q->frontIndex = (q->frontIndex + 1)%q->capacity;
	
	return item;
}

/**
 * Gets the item at the front of the queue without removing it
 * Assumes that the queue is not empty
 */
Item QueueFront(Queue q) {
	assert(q->size > 0);
    //printf("q->items[q->frontIndex]: %d\n",q->items[q->frontIndex]);//remove
	return q->items[q->frontIndex];
}

/**
 * Gets the size of the given queue
 */
int QueueSize(Queue q) {
	return q->size;
}

/**
 * Returns true if the queue is empty, and false otherwise
 */
bool QueueIsEmpty(Queue q) {
	return q->size == 0;
}

/**
 * Prints the queue to the given file with items space-separated
 */
void QueueDump(Queue q, FILE *fp) {
	for (int i = q->frontIndex, j = 0; j < q->size; i = (i + 1) % q->capacity, j++) {
		fprintf(fp, "%d ", q->items[i]);
	}
	fprintf(fp, "\n");
}

/**
 * Prints out information for debugging
 */
void QueueDebugPrint(Queue q) {
    for(int i = q->frontIndex, j = 0; j < q->size; i = (i + 1) % q->capacity, j++){
        printf("Index %d Val:%d,Size: %d, Front Index: %d, Capacity: %d \n",i,q->items[i],q->size,q->frontIndex,q->capacity);
    }
    
}
void ass_test(Queue q,int i){
    if(q->items[(q->frontIndex + i)%q->capacity] != i){
        fprintf(stderr, "fix realloc?");
		exit(EXIT_FAILURE);
    }
}

