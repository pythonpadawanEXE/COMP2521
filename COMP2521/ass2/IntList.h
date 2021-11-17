// IntList.h - Lists of integers (interface)

#ifndef INTLIST_H
#define INTLIST_H //TODO: what does the ifndef and define do in this case

#include <stdbool.h>
#include <stdio.h>


// data structures representing IntList
struct IntListNode {
	double data;
	// pointer to next node
	struct IntListNode *next; 
};

struct IntListRep {
	// number of elements in the list
	int size;
	// pointer to node containing first value                  
	struct IntListNode *first;
	// pointer to node containing last value 
	struct IntListNode *last;  
};
/**
 * External view of IntList ... implementation in IntList.c
 */
typedef struct IntListRep *IntList;



/**
 * Create a new, empty IntList.
 */
IntList IntListNew(void);

/**
 * Release all resources associated with an IntList.
 */
void IntListFree(IntList l);

/**
 * Create an IntList by reading values from a file.
 * Assume that the file is open for reading.
 */
IntList IntListRead(FILE *in);

/**
 * Display IntList as one integer per line on `stdout`.
 */
void IntListShow(IntList l);

/**
 * Append one integer to the end of an IntList.
 */
void IntListAppend(IntList l, double v);

/**
 * Insert an integer into correct place in a sorted IntList.
 */
void IntListInsertInOrder(IntList l, double v);

/**
 * Return number of elements in an IntList.
 */
int IntListLength(IntList l);

/**
 * Make a copy of an IntList.
 * New list should look identical to the original list.
 */
IntList IntListCopy(IntList l);

/**
 * Make a sorted copy of an IntList.
 */
IntList IntListSortedCopy(IntList l);

/**
 * Check whether an IntList is sorted in ascending order.
 * Returns `false` if list is not sorted, `true` if it is.
 */
bool IntListIsSorted(IntList l);

/**
 * Check internal consistency of an IntList.
 */
bool IntListOK(IntList l);

/**
 * Display an IntList as one integer per line to a file.
 * Assume that the file is open for writing.
 */
void IntListPrint(FILE *out, IntList l);


#endif
