// Implementation of the Queue ADT using a linked list
//z5165158 26/09/2021

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Queue.h"

typedef struct node *Node;
struct node {
	Item item;
	Node next;
};

struct queue {
	Node head;
	Node tail;
	int  size;
};

/**
 * Creates a new empty queue
 */
Queue QueueNew(void) {
	Queue q = malloc(sizeof(*q));
	if (q == NULL) {
		fprintf(stderr, "couldn't allocate Queue\n");
		exit(EXIT_FAILURE);
	}

	q->head = NULL;
	q->tail = NULL;
	q->size = 0;
	return q;
}

/**
 * Frees all resources associated with the given queue
 */
void QueueFree(Queue q) {
	Node curr = q->head;
	while (curr != NULL) {
		Node temp = curr;
		curr = curr->next;
		free(temp);
	}
	free(q);
}

/**
 * Adds an item to the end of the queue
 */
void QueueEnqueue(Queue q, Item it) {
	// TODO: MARK
	Node new_node = malloc(sizeof(Node));
	if (new_node == NULL) {
		fprintf(stderr, "couldn't allocate Node");
		exit(EXIT_FAILURE);
	}
	new_node->next = NULL;
	new_node->item = it;
	//if list is empty
	if(q-> head == NULL || q->size == 0){
	    q->head = new_node;
	    q->tail = new_node;
	    q->size++;
	}
	//if list not empty
	else {
	    new_node->item = it;
	    q->tail->next = new_node;
	    q->tail = new_node;
	    q->size++;
	}
}

/**
 * Removes an item from the front of the queue and returns it
 * Assumes that the queue is not empty
 */
Item QueueDequeue(Queue q) {
    // TODO: MARK
    
    //How should an empty list be tested for dequeue?
    if(q-> head == NULL || q->size == 0){
        fprintf(stderr, "couldn't Dequeue list appears empty");
		exit(EXIT_FAILURE);
    }
	Item item = q->head->item;
	Node temp = q->head;
	q->head = q->head->next;
	q->size--;
	free(temp);
	
	return item;
}

/**
 * Gets the item at the front of the queue without removing it
 * Assumes that the queue is not empty
 */
Item QueueFront(Queue q) {
	assert(q->size > 0);

	return q->head->item;
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
	for (Node curr = q->head; curr != NULL; curr = curr->next) {
		fprintf(fp, "%d ", curr->item);
	}
	fprintf(fp, "\n");
}

/**
 * Prints out information for debugging
 */
void QueueDebugPrint(Queue q) {
    int num = 0;
    for(Node curr = q->head; curr != NULL;curr = curr->next){
        printf("Node %d:%d\n",num,curr->item);
        num++;
    }
}
Node create_node(int value){
    Node new_node = malloc(sizeof(Node));
    new_node->next = NULL;
	new_node->item = value;
}

Node recursion(Node curr){
    if (curr == NULL){
        return NULL;
    }
    if (curr->next == NULL)
        return curr;
    Node list2 = recursion(curr->next);
    curr->next->next = curr;
    curr->next = NULL;
    return list2;
}
Node append(Node list, int value) {
    if (list == NULL) {
        return create_node(value);
    } else {
        append(list->next, value);
        return list;
    }
}

int main(void){
    Queue q = QueueNew();
    //QueueEnqueue(q,1);
    Node list = append(q->head, 2);
    QueueDebugPrint(q);
    /*
    int num = 0;
    for(Node curr = recursion(q->head); curr != NULL;curr = curr->next){
        printf("Node %d:%d\n",num,curr->item);
        num++;
    }
    */
    return 0;
 }
