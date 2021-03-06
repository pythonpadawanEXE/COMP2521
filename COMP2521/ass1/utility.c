// COMP2521 Assignment 1

// Written by:Jake Edwards
// Date: 24/10/2021

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FileType.h"
#include "utility.h"

struct tree {
		char *canonical_path;
		char *curr_path;
		char *parent_path;
		char *content;
		int  children_size;
		FileType tree_type;
		Tree curr_tree;
		Tree parent;
		Tree children_head;
		Tree children_tail;
		Tree next;
		//NOT IN USE
		Tree head;
		
};
struct str_node {
	Item item;
	STR_Node next;
};
struct string_queue{
	STR_Node head;
	STR_Node tail;
	int  size;
};

/*



START OF QUEUE FUNCTIONS
(TAKEN FROM LAB2)




*/

Queue QueueNew(void) {
	Queue q = malloc(sizeof(struct string_queue));
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
        free(temp->item);
		free(temp);
	}
	free(q);
}

/**
 * Adds an item to the end of the queue
 */
void QueueEnqueue(Queue q, Item it) {
	// TODO: MARK
	Node new_node = malloc(sizeof(struct str_node));
	if (new_node == NULL) {
		fprintf(stderr, "couldn't allocate Node");
		exit(EXIT_FAILURE);
	}
	new_node->next = NULL;
	new_node->item = malloc((PATH_MAX+1)*sizeof(char));
    strcpy(new_node->item,it);
	//if list is empty
	if(q-> head == NULL || q->size == 0){
			q->head = new_node;
			q->tail = new_node;
			q->size++;
	}
	//if list not empty
	else {
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
		fprintf(fp, "%s ", curr->item);
	}
	fprintf(fp, "\n");
}
/*



START OF TREE FUNCTIONS





*/

//make new tree with defined paths 
Tree GetNewTree(char *canonical_path,char* curr_path,char *parent_path,FileType tree_type,Tree parent_tree){
		Tree NewTree = malloc(sizeof(struct tree));
		if (NewTree == NULL) {
				fprintf(stderr, "error: out of memory\n");
				exit(EXIT_FAILURE);
		}
		
		char *canonical_path_alloc = canonical_path;
		char *curr_path_alloc = curr_path;
		char *parent_path_alloc = parent_path;
		
		NewTree->canonical_path = canonical_path_alloc;
		NewTree->curr_path = curr_path_alloc;
		NewTree->parent_path = parent_path_alloc;
		NewTree->content = NULL;

		NewTree->children_size = 0;
		NewTree->tree_type = tree_type;

		NewTree->curr_tree = NewTree;
		NewTree->parent = parent_tree;

		NewTree->children_head = NULL;
		NewTree->children_tail = NULL;
	
		NewTree->next = NULL;

		
		NewTree->head = NULL;
		

		return NewTree;
}
//add a tree in alphabetical order at path name tail address
void AddTree(Tree path_addr,Tree NewTree,char *err_msg,char *path,char *path_name_tail){
	Tree tree_parent = path_addr;
	NewTree->parent = path_addr;
	if(strcmp(path_name_tail,"/.")==0){
		printf("%s '%s': File exists\n",err_msg,path);
		FsFreeTree(NewTree);
		NewTree = NULL;
		return;
	}
	else if(strcmp(path_name_tail,"/..")==0){
		printf("%s '%s': File exists\n",err_msg,path);
		FsFreeTree(NewTree);
		NewTree = NULL;
		return;
	}
	else if(strcmp(path_name_tail,"")==0){
		printf("%s '%s': File exists\n",err_msg,path);
		FsFreeTree(NewTree);
		NewTree = NULL;
		return;
	}
	
	
	ChildrenTreeListInsertInOrder(tree_parent,NewTree,err_msg,path);
	
}
//add/replace string within tree content if the tree tail matches the path tail
void PutContent(Tree path_addr,char *err_msg,char *path,char *path_name_tail,char *content){
	
	Tree head = path_addr->children_head;
	
	while (head != NULL){
		if(strcmp(head->curr_path,path_name_tail) == 0 && head->tree_type == REGULAR_FILE){
			if(head->content != NULL){
				free(head->content);
				head->content = NULL;
			}
			head->content = malloc((strlen(content)+1)*sizeof(char));
			assert(head->content != NULL);
			strcpy(head->content,content);
			return;
		}
		if(strcmp(head->curr_path,path_name_tail) == 0 && head->tree_type == DIRECTORY){
			printf("%s '%s': Is a directory\n",err_msg,path);
			return;
		}
		head = head->next;
	}
	if(strcmp(path_addr->curr_path,path_name_tail) == 0 && path_addr->tree_type == REGULAR_FILE){
			if(path_addr->content != NULL){
				free(path_addr->content);
				path_addr->content = NULL;
			}
			path_addr->content = malloc((strlen(content)+1)*sizeof(char));
			assert(path_addr->content != NULL);
			strcpy(path_addr->content,content);
			return;
	}
	else if(strcmp(path_name_tail,"/.") == 0 || strcmp(path_name_tail,"/..")){
		printf("%s '%s': Is a directory\n",err_msg,path);
		return;
	}
	printf("%s '%s': No such file or directory\n",err_msg,path);
}
//print string within tree content if the tree tail matches the path tail
void CatContent(Tree path_addr,char *err_msg,char *path,char *path_name_tail){
	
	Tree head = path_addr->children_head;
	
	while (head != NULL){
		if(strcmp(head->curr_path,path_name_tail) == 0 && head->tree_type == REGULAR_FILE){
			if(head->content != NULL){
				printf("%s",head->content);
			}
			return;
		}
		if(strcmp(head->curr_path,path_name_tail) == 0 && head->tree_type == DIRECTORY){
			printf("%s '%s': Is a directory\n",err_msg,path);
			return;
		}
		head = head->next;
	}
	if(strcmp(path_addr->curr_path,path_name_tail) == 0 && path_addr->tree_type == REGULAR_FILE){
			if(path_addr->content != NULL){
				printf("%s",path_addr->content);
			}
			return;
	}
	else if(strcmp(path_name_tail,"/.") == 0 || strcmp(path_name_tail,"/..")){
		printf("%s '%s': Is a directory\n",err_msg,path);
		return;
	}
	printf("%s '%s': No such file or directory\n",err_msg,path);
}
//deletes directory if empty
void DlDir(Tree path_addr,char *err_msg,char *path,char *path_name_tail){
	
	Tree head = path_addr->children_head;
	Tree prev = NULL;
	
	while (head != NULL){

		//DELETE IF DIRECTORY MATCHES AND IS EMPTY
		if(strcmp(head->curr_path,path_name_tail) == 0 && head->tree_type == DIRECTORY && head->children_head == NULL){
			
			if(prev != NULL){
				prev->next = head->next;
				FsFreeTree(head);
			}
			else if(prev == NULL){
				
				path_addr->children_head = head->next;
				FsFreeTree(head);
			}
			return;
		}
		else if(strcmp(head->curr_path,path_name_tail) == 0 && head->tree_type == DIRECTORY && head->children_head != NULL){
			printf("%s '%s': Directory not empty\n",err_msg,path);
			return;
		}
		else if(strcmp(head->curr_path,path_name_tail) == 0 && head->tree_type == REGULAR_FILE){
			printf("%s '%s': Not a directory\n",err_msg,path);
			return;
		}
		prev = head;
		head = head->next;
	}
	if(strcmp(path_addr->curr_path,path_name_tail) == 0 && path_addr->tree_type == DIRECTORY && path_addr->children_head == NULL){
		head = path_addr;
		while (head != NULL){

			//DELETE IF DIRECTORY MATCHES AND IS EMPTY
			if(strcmp(head->curr_path,path_name_tail) == 0 && head->tree_type == DIRECTORY && head->children_head == NULL){
				
				if(prev != NULL){
					prev->next = head->next;
					FsFreeTree(head);
				}
				else if(prev == NULL){
					
					path_addr->children_head = head->next;
					FsFreeTree(head);
				}
				return;
			}
			else if(strcmp(head->curr_path,path_name_tail) == 0 && head->tree_type == DIRECTORY && head->children_head != NULL){
				printf("%s '%s': Directory not empty\n",err_msg,path);
				return;
			}
			else if(strcmp(head->curr_path,path_name_tail) == 0 && head->tree_type == REGULAR_FILE){
				printf("%s '%s': Not a directory\n",err_msg,path);
				return;
			}
			prev = head;
			head = head->next;
		}		
		
	}
	else if(strcmp(path_name_tail,"/.") == 0){
		printf("%s '%s': Invalid Argument\n",err_msg,path);
		return;
	}
	
	else if(strcmp(path_name_tail,"/..") == 0){
		printf("%s '%s': Directory not empty\n",err_msg,path);
		return;
	}
	printf("%s '%s': No such file or directory\n",err_msg,path);
}
//recusrively delete if activated otherwise just delete
void Dl(Tree path_addr,char *err_msg,char *path,char *path_name_tail,bool recursive){
	
	Tree head = path_addr->children_head;
	Tree prev = NULL;
	
	while (head != NULL){

		//DELETE IF DIRECTORY MATCHES AND IS EMPTY
		if(strcmp(head->curr_path,path_name_tail) == 0 && recursive == true){
			
			if(prev != NULL){
				prev->next = head->next;
				FsFreeTree(head->children_head);
				
				FreeTree(head);
			}
			else if(prev == NULL){
				
				path_addr->children_head = head->next;
				FsFreeTree(head->children_head);
				
				FreeTree(head);
				
			}
			return;
		}
		else if(strcmp(head->curr_path,path_name_tail) == 0 && head->tree_type == DIRECTORY && recursive == false){
			printf("%s '%s': Is a directory\n",err_msg,path);
			return;
		}
		prev = head;
		head = head->next;
	}
	
	if(strcmp(path_name_tail,"/.") == 0){
		printf("%s '%s': Invalid Argument\n",err_msg,path);
		return;
	}
	
	else if(strcmp(path_name_tail,"/..") == 0){
		printf("%s '%s': Directory not empty\n",err_msg,path);
		return;
	}
	printf("%s '%s': No such file or directory\n",err_msg,path);
}


//Prints list of trees while head is not null
void PrintLs(Tree tail_tree,char *err_msg,char *path){
	
	if(tail_tree  == NULL){
		
		return;
	}
	if(tail_tree->tree_type == REGULAR_FILE){
		printf("%s\n",path);
		
		return;
	}
	
	Tree head = tail_tree->children_head;
	//MAGIC NUMBER 1 to offset prepended slash
	while (head != NULL){
		printf("%s\n",head->curr_path + 1);
		head = head->next;
	}
	

}
//prints the tree recursively
void PrintTreeRecur(Tree tree,int indent){
	if (tree != NULL) {
		for(int i = 0; i < indent; i ++){
			printf(" ");
		}
		if(strcmp(tree->curr_path,ROOT_PATH) != 0){
        	printf("%s\n",tree->curr_path + strlen("/"));
		}
		else {
			printf("/\n");
		}
		PrintTreeRecur(tree->children_head,indent + 4);
        PrintTreeRecur(tree->next,indent);
    }
}


// void PrintTreeRecur(Tree tree,int indent){
// 	if (tree != NULL) {
// 		for(int i = 0; i < indent; i ++){
// 			printf(" ");
// 		}
// 		if(strcmp(tree->curr_path,ROOT_PATH) != 0){
//         	printf("%s\n",tree->curr_path + strlen("/"));
// 		}
// 		else {
// 			printf("/\n");
// 		}
// 		PrintTreeRecur(tree->children_head,indent + 4);
//         PrintTreeRecur(tree->next,indent);
//     }
// }





// return tree for path_name tail using tree from return treedir  i.e. use tree address from one level above the tail to find the tail
Tree ReturnTreeFomTail(Tree path_addr,char *path_name_tail,char *err_msg,char *path,Queue Q_PATH,ModeType Mode){
	//some condition when path_addr is  root i.e. head is tail
	if(Q_PATH->size == 1){
		return path_addr;
	}
	if(strcmp(path_name_tail,"/.") == 0){
		return path_addr;
	}
	if(strcmp(path_name_tail,"/..") == 0){
		if(path_addr->parent != NULL){
			return path_addr->parent;
		}
		//this should only ever return root
		else {
			return path_addr;
		}
	}
	Tree head = path_addr->children_head;
	while (head != NULL){
		if(strcmp(head->curr_path,path_name_tail) == 0 && head->tree_type == DIRECTORY){
			return head;
		}
		if(strcmp(head->curr_path,path_name_tail) == 0 && head->tree_type == REGULAR_FILE){
			if(Mode != LS_MODE){
				printf("%s '%s': Not a directory\n",err_msg,path);
				return NULL;
			}
			printf("%s\n",path);
			return NULL;
		}
		head = head->next;
	}
	
	printf("%s '%s': No such file or directory\n",err_msg,path);
	return NULL;
	
}

char *GetParentPath(Tree path_addr){
	return path_addr->canonical_path;
}

char *GetCanonicalPath(Tree CWD){
	return CWD->canonical_path;
}
Tree GetRootChildrenHead(Tree tree){
	return tree->children_head;
}
void  FreeTree(Tree tree){
	if(tree->curr_path != NULL){
		free(tree->curr_path);
		tree->curr_path = NULL;
	}
	if(tree->canonical_path != NULL){
		free(tree->canonical_path);
		tree->canonical_path = NULL;
	}
	if(tree->content != NULL){
		free(tree->content);
		tree->content = NULL;
	}
	free(tree);
}

void FreeRoot(Tree tree){
	if(tree->curr_path != NULL){
		free(tree->curr_path);
		tree->curr_path = NULL;
	}
	if(tree->parent_path != NULL){
		free(tree->parent_path);
		tree->parent_path = NULL;
	}
	if(tree->canonical_path != NULL){
		free(tree->canonical_path);
		tree->canonical_path = NULL;
	}
	if(tree->content != NULL){
		free(tree->content);
		tree->content = NULL;
	}
	free(tree);
}

void FsFreeTree(Tree tree) {
    if (tree != NULL) {
        FsFreeTree(tree->children_head);
        FsFreeTree(tree->next);
        
		if (tree->curr_path != NULL) {
            free(tree->curr_path);
			tree->curr_path = NULL;
        }
		if (tree->parent_path != NULL) {
			tree->parent_path = NULL;
        }
		if (tree->canonical_path != NULL) {
            free(tree->canonical_path);
			tree->canonical_path = NULL;
        }
		if(tree->content != NULL){
			free(tree->content);
			tree->content = NULL;
		}
        free(tree);
		tree = NULL;
    }
	// return;
}

//Returns a tree to the directory one level above the path name tail
Tree ReturnTreeDir(Tree Root,Tree tree, char *path,char *err_msg,Queue Q_PATH,STR_Node path_part,ModeType Mode){
	if(Q_PATH->size == 1){
		return NULL;
	}
	if(Q_PATH->size == 2){
		return tree;
	}
	
	if(strcmp("/",path) == 0 || strcmp("",path) ==0  ){
		return  Root;
	}
	
	while(path_part->next != NULL && tree != NULL){
		
		//IF MATCH BUT NOT A DIRECTORY
		if(strcmp(tree->curr_path,path_part->item) == 0 && tree->tree_type == REGULAR_FILE){
			
			printf("%s '%s': Not a directory\n",err_msg,path);				
			return NULL;
		}
		else if(strcmp("/.",path_part->item) == 0){

			tree = tree;
			path_part = path_part->next;
			if(tree == NULL && path_part->next == NULL){
				return Root;
			}
			
		}
		else if(strcmp("/..",path_part->item) == 0){
			if(tree->parent == NULL){
				return Root;
			}
			tree = tree->parent;
			path_part = path_part->next;
			//CONDITIONS TO GO BACK TO ROOT e.g. input is ls ../ at Root
			if(tree == NULL && path_part->next == NULL){
				return Root;
			}
			
			
		}
		
		//IF MATCH AND IS DIRECTORY
		else if(strcmp(tree->curr_path,path_part->item) == 0 && tree->tree_type == DIRECTORY){
			
			
			if(strcmp(path_part->next->item,"/..")==0){
				path_part = path_part->next;
			}
			else if(strcmp(path_part->next->item,"/.") == 0){
				if(tree == Root){
					tree = tree->children_head;
				}
				path_part = path_part->next;
			}
			else if (path_part->next->next == NULL){
				return tree;
			}
			else{
				tree = tree->children_head;
				path_part = path_part->next;
			}
		}
		//IF  NOT MATCH AND NOT EXISTS NEXT TREE 
		else if(strcmp(tree->curr_path,path_part->item) != 0 && tree->next == NULL){
			
			printf("%s '%s': No such file or directory\n",err_msg,path);
			return NULL;
		}
		//IF NOT MATCH AND  EXISTS NEXT TREE   
		else if(strcmp(tree->curr_path,path_part->item) != 0 && tree->next != NULL){
			
			tree = tree->next;
		}
		else{
			printf("unaccounted for scenario\n");
			return NULL;
		}
	}
	if(tree == NULL){
		printf("%s '%s': No such file or directory\n",err_msg,path);
	}
	return tree;
}


void PrintCanonicalPath(Tree tree){
	printf("%s\n",tree->canonical_path);
}
STR_Node GetQPathHead(Queue Q_PATH) {
	return Q_PATH->head;
}

char* GetPathNameTail(Queue Q_PATH){
	return Q_PATH->tail->item;
}

bool check_is_directoryname(char* token){
	if(strchr(token,'.') != NULL){
		return false;
	}
	return true;
}
//https://stackoverflow.com/questions/2328182/prepending-to-a-string
void prepend(char* s, const char* t){
    size_t len = strlen(t);
    memmove(s + len, s, strlen(s) + 1);
    memcpy(s, t, len);
}

//Create a Queue containing strings in each Queue Node for the parts of a file path
Queue ReturnQPath(char *path,Tree CWD){
		Queue q = QueueNew();
		QueueEnqueue(q,CWD->curr_path);	
		const char s[2] = "/";
		char *token;
        
		//separate path via / delimiter
        char *tmp_path = malloc((PATH_MAX+1)*sizeof(char));
		char *tmp_dir = malloc((PATH_MAX+1)*sizeof(char));
		if (tmp_path == NULL) {
				fprintf(stderr, "error: out of memory\n");
				exit(EXIT_FAILURE);
		}
        strcpy(tmp_path,path);
		strcpy(tmp_path,path);
		token = strtok(tmp_path, s);
		
		
		//loop separating path via / delimiter
		while(token != NULL ) {
            if(token != NULL){
				strcpy(tmp_dir,token);
				prepend(tmp_dir,"/");
				QueueEnqueue(q,tmp_dir);
				
            }
			token = strtok(NULL, s);
		}
		free(tmp_dir);
        free(tmp_path);
		return q;   
}

/**
 * Append one tree to the end of  tree_parent children.
 */
void ChildrenTreeListAppend(Tree tree_parent, Tree NewTree) {
	
	if (tree_parent->children_head == NULL) {
		tree_parent->children_head = NewTree;
		tree_parent->children_tail = NewTree;
	} else {
		tree_parent->children_tail->next = NewTree;
		tree_parent->children_tail = NewTree;
	}
	tree_parent->children_size++;
}

/**
 * Prepend one tree to the start of  tree_parent children.
 */
void ChildrenTreeListPrepend(Tree tree_parent, Tree NewTree) {
	if (tree_parent->children_head == NULL) {
		tree_parent->children_head = tree_parent->children_tail = NewTree;
	} else {
		NewTree->next = tree_parent->children_head;
		tree_parent->children_head = NewTree;
	}
	tree_parent->children_size++;
}

/**
 * Insert one tree into correct place in a sorted TreeList.
 */
void ChildrenTreeListInsertInOrder(Tree tree_parent, Tree NewTree,char *err_msg,char *path) {
	//if empty list
	if (tree_parent->children_head == NULL) {
		ChildrenTreeListAppend(tree_parent,NewTree);
	}
	//check if file exists at head
	else if(strcmp(NewTree->curr_path,tree_parent->children_head->curr_path) == 0 ){
		printf("%s '%s': File exists\n",err_msg,path);
		FsFreeTree(NewTree);
		return;
	}
	//check if file exists at tail
	else if(strcmp(NewTree->curr_path,tree_parent->children_tail->curr_path) == 0 ){
		printf("%s '%s': File exists\n",err_msg,path);
		FsFreeTree(NewTree);
		return;
	}
	//prepend if New Tree alphabetically before current head
	else if (strcmp(NewTree->curr_path,tree_parent->children_head->curr_path) < 0){
		ChildrenTreeListPrepend(tree_parent,NewTree);
	}
	
	//if integer inserted at end i.e. append
	else if (strcmp(NewTree->curr_path,tree_parent->children_tail->curr_path) > 0 ){
		ChildrenTreeListAppend(tree_parent,NewTree);
	}
	
	//integer inserted in middle of list
	else {
		Tree n = NewTree;
		Tree curr = tree_parent->children_head ;
		while( curr->next != NULL){
			//if new directory is alphabetically between next directory and current directory
			if(strcmp(NewTree->curr_path,curr->next->curr_path) < 0 && strcmp(NewTree->curr_path,curr->curr_path) > 0){
				n->next = curr->next;
				curr->next = n;
				break;
			}
			else if(strcmp(NewTree->curr_path,curr->curr_path) == 0 ){
				printf("%s '%s': File exists\n",err_msg,path);
				FsFreeTree(NewTree);
				return;
			}
			curr = curr->next;
		}

		tree_parent->children_size++;
	}
	
}


