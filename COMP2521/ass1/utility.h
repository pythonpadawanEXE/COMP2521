// COMP2521 Assignment 1

// Written by:  Jake Edwards
// Date:    24/10/2021

#ifndef UTILITY_H
#define UTILITY_H
#include "Fs.h"
// add your own #includes, #defines, typedefs, structs and
// function prototypes here

typedef enum {
    MK_MODE,
    LS_MODE,
    CD_MODE,
    FS_MODE,
} ModeType;

#define CHILDREN_SIZE 20
#define ROOT_PATH "/(Root)"
typedef struct tree *Tree;
//typedef struct file *File;
typedef struct string_queue *STR_Q;
typedef struct str_node *STR_Node;
typedef STR_Q Queue;
typedef STR_Node Node;
typedef char* Item;



//TREE FUNCTIONS
void FsFreeTree(Tree tree);
void AddTree(Tree path_addr,Tree NewTree,char *err_msg,char *path,char *path_name_tail);
void prepend(char* s, const char* t);
void PrintCanonicalPath(Tree tree);
void PrintLs(Tree tail_tree,char *err_msg,char *path);
void PrintTreeRecur(Tree tree,int indent);
void FreeRoot(Tree tree);
void PutContent(Tree path_addr,char *err_msg,char *path,char *path_name_tail,char *content);
void CatContent(Tree path_addr,char *err_msg,char *path,char *path_name_tail);
void DlDir(Tree path_addr,char *err_msg,char *path,char *path_name_tail);
void Dl(Tree path_addr,char *err_msg,char *path,char *path_name_tail,bool recursive);
void  FreeTree(Tree tree);
Tree ReturnPathFromCur(Tree Root,char* token,Tree cur,char *path,char *err_msg);
//input needs to change
Tree GetNewTree(char *canonical_path,char* curr_path,char *parent_path,FileType tree_type,Tree parent_tree);
Tree ReturnTreeDir(Tree Root,Tree tree, char *path,char *err_msg,Queue Q_PATH,STR_Node path_part,ModeType Mode);
Tree ReturnTreeFomTail(Tree path_addr,char *path_name_tail,char *err_msg,char *path,Queue Q_PATH,ModeType Mode);
Tree GetRootChildrenHead(Tree tree);
bool check_is_directoryname(char* token);

Queue ReturnQPath(char *path,Tree CWD);

char *GetPathNameTail(Queue Q_PATH);
char *GetCanonicalPath(Tree CWD);
char *GetParentPath(Tree path_addr);

//TREE CHILDREN ADDING
void ChildrenTreeListAppend(Tree tree_parent, Tree NewTree);
void ChildrenTreeListPrepend(Tree tree_parent, Tree NewTree);
void ChildrenTreeListInsertInOrder(Tree tree_parent, Tree NewTree,char *err_msg,char *path);

STR_Node GetQPathHead(Queue Q_PATH); 

/**
 * Creates a new empty queue
 */
Queue QueueNew(void);

/**
 * Frees all resources associated with the given queue
 */
void QueueFree(Queue q);

/**
 * Adds an item to the end of the queue
 */
void QueueEnqueue(Queue q, Item it);

/**
 * Removes an item from the front of the queue and returns it
 * Assumes that the queue is not empty
 */
Item QueueDequeue(Queue q);

/**
 * Gets the item at the front of the queue without removing it
 * Assumes that the queue is not empty
 */
Item QueueFront(Queue q);

/**
 * Gets the size of the given queue
 */
int QueueSize(Queue q);

/**
 * Returns true if the queue is empty, and false otherwise
 */
bool QueueIsEmpty(Queue q);

/**
 * Prints the queue to the given file with items space-separated
 */
void QueueDump(Queue q, FILE *fp);

/**
 * Prints out information for debugging
 */
void QueueDebugPrint(Queue q);
#endif