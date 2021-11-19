// List.h - Lists of integers (interface)

#ifndef List_H
#define List_H //TODO: what does the ifndef and define do in this case

#include <stdbool.h>
#include <stdio.h>
#include <string.h>


// data structures representing List
struct ListNode {
	double data;
	char *str;
	// pointer to next node
	struct ListNode *next; 
};

struct ListRep {
	// number of elements in the list
	int size;
	// pointer to node containing first value                  
	struct ListNode *first;
	// pointer to node containing last value 
	struct ListNode *last;  
};
/**
 * External view of List ... implementation in List.c
 */
typedef struct ListRep *List;



/**
 * Create a new, empty List.
 */
List ListNew(void);

/**
 * Release all resources associated with an List.
 */
void ListFree(List l);

/**
 * Create an List by reading values from a file.
 * Assume that the file is open for reading.
 */
List ListRead(FILE *in);

/**
 * Display List as one integer per line on `stdout`.
 */
void ListShow(List l);

/**
 * Append one integer to the end of an List.
 */
void ListAppend(List l, double v,char *str);

/**
 * Insert an integer into correct place in a sorted List.
 */
void ListInsertInOrder(List l, double v,char *str);

/**
 * Return number of elements in an List.
 */
int ListLength(List l);

/**
 * Make a copy of an List.
 * New list should look identical to the original list.
 */
List ListCopy(List l);

/**
 * Make a sorted copy of an List.
 */
List ListSortedCopy(List l);

/**
 * Check whether an List is sorted in ascending order.
 * Returns `false` if list is not sorted, `true` if it is.
 */
bool ListIsSorted(List l);

/**
 * Check internal consistency of an List.
 */
bool ListOK(List l);

/**
 * Display an List as one integer per line to a file.
 * Assume that the file is open for writing.
 */
void ListPrint(FILE *out, List l);

void ListInsertRevInOrder(List l, double v,char *str);
void ListPrepend(List l, double v, char *str) ;
#endif
