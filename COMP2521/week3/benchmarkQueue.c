
#include <stdio.h>
#include <stdlib.h>

#include "Queue.h"

int main(void) {
	Queue q = QueueNew();

	// TODO
	// Write a benchmark test to demonstrate the difference between
	// ArrayQueue and CircularArrayQueue
    int size = 100000;
    for(int i =0 ; i < size; i++){
        QueueEnqueue(q,i);
    }
    for(int i =0 ; i < size-1; i++){
        QueueDequeue(q);

    }


	QueueFree(q);
}

