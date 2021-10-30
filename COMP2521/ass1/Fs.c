// Implementation of the File System ADT
// COMP2521 Assignment 1

// Written by: Jake Edwards
// Date:24/10/2021

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FileType.h"
#include "Fs.h"
#include "utility.h"


struct FsRep {
    Tree Root;
    Tree CWD;
};


Fs FsNew(void) {
    Fs NewFs = malloc(sizeof(struct FsRep));
    if (NewFs == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    //make paths "/(root)" i.e. ROOT_PATH and when printing add / prepended to each directory
    
    char * RootPath = malloc((PATH_MAX+1)*sizeof(char));
    char * RootPath1 = malloc((PATH_MAX+1)*sizeof(char));
    char * RootPath2 = malloc((PATH_MAX+1)*sizeof(char));
    strcpy(RootPath,ROOT_PATH);
    strcpy(RootPath1,ROOT_PATH);
    strcpy(RootPath2,ROOT_PATH);
    Tree NewTree = GetNewTree(RootPath,RootPath1,RootPath2,DIRECTORY,NULL);
    NewFs->Root = NewTree;
    NewFs->CWD = NewTree;
    return NewFs;
}

void FsGetCwd(Fs fs, char cwd[PATH_MAX + 1]) {
    char *str = GetCanonicalPath(fs->CWD);
    if(strcmp(str,ROOT_PATH) == 0){
        strncpy(cwd,str,strlen("/"));
    }
    else{
        strcpy(cwd,str +strlen(ROOT_PATH));
    }

}

//free recursively all trees
void FsFree(Fs fs) {
   
    
    if(fs->Root != NULL){
        
        Tree tree = GetRootChildrenHead(fs->Root);
        FsFreeTree(tree);
        FreeRoot(fs->Root);
        fs->Root = NULL;
        free(fs);
        fs = NULL;
    }
     
}
//Insert Directory into path
void FsMkdir(Fs fs, char *path) {
    
        
    //Return Path check for errors and handle finding path
    char *err_msg = "mkfile: cannot create directory";
    char *path_name_tail = malloc((PATH_MAX+1)*sizeof(char));
    Tree tree = NULL;
    if(strncmp(path,"/",strlen("/")) == 0){
        tree = fs->Root;
    }
    else {
        tree = fs->CWD;
    }
    
    //Q path is a q of strings delemiting the directory path
    Queue Q_PATH = ReturnQPath(path,tree);
    //assign the tailing directory name to path_name_tail
    strcpy(path_name_tail,GetPathNameTail(Q_PATH));
    
    STR_Node path_part = GetQPathHead(Q_PATH);

    Tree path_addr = ReturnTreeDir(fs->Root,tree,path,err_msg,Q_PATH,path_part,MK_MODE);

    //make new subtree at path_addr as a directory
    if(path_addr != NULL){
        
        
        char *parent_path = GetParentPath(path_addr);
        char *canonical_path = malloc((PATH_MAX+1)*sizeof(char));
        char *safe_canonical_path = malloc((PATH_MAX+1)*sizeof(char));
        assert(canonical_path != NULL);
        assert(safe_canonical_path != NULL);

        
        
        strcpy(canonical_path,parent_path);
        strcat(canonical_path,path_name_tail);
        strcpy(safe_canonical_path,canonical_path);
        //make new tree
        Tree NewTree = GetNewTree(safe_canonical_path,path_name_tail,parent_path,DIRECTORY,path_addr);
        //add new tree in alphabetical order inside path_address children if it matches a name at the same level free tree and print error of file exists
        AddTree(path_addr,NewTree,err_msg,path,path_name_tail);
        free(canonical_path);
        canonical_path = NULL;
    }
    else {
        free(path_name_tail);
        path_name_tail = NULL;
    }
    QueueFree(Q_PATH);
    Q_PATH = NULL;
}

void FsMkfile(Fs fs, char *path) {
    
    
    char *err_msg = "mkfile: cannot create file";
    char *path_name_tail = malloc((PATH_MAX+1)*sizeof(char));
    Tree tree = NULL;
    if(strncmp(path,"/",strlen("/")) == 0){
        tree = fs->Root;
    }
    else {
        tree = fs->CWD;
    }
    //Q path is a q of strings delemiting the directory path
    Queue Q_PATH = ReturnQPath(path,tree);
    //assign the tailing directory name to path_name_tail
    strcpy(path_name_tail,GetPathNameTail(Q_PATH));
    
    STR_Node path_part = GetQPathHead(Q_PATH);
    //Return Path check for errors and handle finding path
    Tree path_addr = ReturnTreeDir(fs->Root,tree,path,err_msg,Q_PATH,path_part,MK_MODE);

    //make new subtree at path_addr as a directory in children_head where path_addr is parent of children_head
    if(path_addr != NULL){
        
        
        char *parent_path = GetParentPath(path_addr);
        char *canonical_path = malloc((PATH_MAX+1)*sizeof(char));
        char *safe_canonical_path = malloc((PATH_MAX+1)*sizeof(char));
        assert(canonical_path != NULL);
        assert(safe_canonical_path != NULL);

        
        
        strcpy(canonical_path,parent_path);
        strcat(canonical_path,path_name_tail);
        strcpy(safe_canonical_path,canonical_path);
        //make new tree
        Tree NewTree = GetNewTree(safe_canonical_path,path_name_tail,parent_path,REGULAR_FILE,path_addr);
        //add new tree in alphabetical order inside path_address children if it matches a name at the same level free tree and print error of file exists
        AddTree(path_addr,NewTree,err_msg,path,path_name_tail);
        free(canonical_path);
        canonical_path = NULL;
    }
    else {
        free(path_name_tail);
        path_name_tail = NULL;
    }
    
    QueueFree(Q_PATH);
    Q_PATH = NULL;
}
//change workings directory to path
void FsCd(Fs fs, char *path) {
    char *err_msg = "cd: ";
    char *path_name_tail = malloc((PATH_MAX+1)*sizeof(char));
    Tree tree = NULL;
    

    Tree path_addr = NULL;
    if (path == NULL){
        fs->CWD = fs->Root;
       
    }
    else{
        if(strncmp(path,"/",strlen("/")) == 0){
            tree = fs->Root;
        }
        else {
            tree = fs->CWD;
        }   
        Queue Q_PATH = ReturnQPath(path,tree);
        STR_Node path_part = GetQPathHead(Q_PATH);
        strcpy(path_name_tail,GetPathNameTail(Q_PATH));
        
        
        if(strcmp(path,"/") == 0 || strcmp(path,"") == 0 ){
            path_addr = fs->Root;
            
        }
        
        else if (path != NULL){
            path_addr = ReturnTreeDir(fs->Root,tree,path,err_msg,Q_PATH,path_part,CD_MODE);
        }
        
        if (path_addr != NULL){
            
            Tree tail_tree = ReturnTreeFomTail(path_addr,path_name_tail,err_msg,path,Q_PATH,CD_MODE);
            if(tail_tree != NULL){
                fs->CWD = tail_tree;
            }
        }
        
        QueueFree(Q_PATH);
        Q_PATH = NULL;
    }
    free(path_name_tail);
    path_name_tail = NULL;
}
// make function for ls from directories at level of path_addr
// go path_addr->children_head then iteratre through for curr_path matching path_name_tail then
// go path_name_tail->children_head and iteratre through printing all trees
// make function for find path name tail directory from directories at level of path_addr
// go path_add->children_head then iteratre through for curr_path matching path_name_tail
void FsLs(Fs fs, char *path) {
    
    char *err_msg = "ls: cannot access ";
    char *path_name_tail = malloc((PATH_MAX+1)*sizeof(char));
    Tree tree = NULL;
    
    Tree path_addr = NULL;
    if (path == NULL){
        path_addr = fs->CWD;
        PrintLs(path_addr,err_msg,path);
    }
    else{
        if(strncmp(path,"/",strlen("/")) == 0){
            tree = fs->Root;
        }
        else {
            tree = fs->CWD;
        }
        Queue Q_PATH = ReturnQPath(path,tree);
        STR_Node path_part = GetQPathHead(Q_PATH);
        strcpy(path_name_tail,GetPathNameTail(Q_PATH));
        
        
        if(strcmp(path,"/") == 0 || strcmp(path,"") == 0 ){
            path_addr = fs->Root;
        }
        
        else if (path != NULL){
            path_addr = ReturnTreeDir(fs->Root,tree,path,err_msg,Q_PATH,path_part,LS_MODE);
        }
        
        if (path_addr != NULL){
            
            Tree tail_tree = ReturnTreeFomTail(path_addr,path_name_tail,err_msg,path,Q_PATH,LS_MODE);
            PrintLs(tail_tree,err_msg,path);
        }
        
        QueueFree(Q_PATH);
        Q_PATH = NULL;
    }
    free(path_name_tail);
    path_name_tail = NULL;
}
/*
The function prints the canonical path of the current working directory.

This function roughly corresponds to the pwd command in Linux.
*/
void FsPwd(Fs fs) {
    
    char *str = GetCanonicalPath(fs->CWD);
    if(strcmp(str,ROOT_PATH) == 0){
        printf("/\n");
    }
    else{
        printf("%s\n",str+strlen(ROOT_PATH));
    }
}
/*
The function takes one path which may be NULL.

If the path is NULL, it defaults to the root directory.

If the path is not NULL, it is expected to refer to a directory and the function 
should print the contents of the directory in a tree-like format (see below).

This function roughly corresponds to the tree command in Linux.
*/
void FsTree(Fs fs, char *path) {
    
    char *err_msg = "tree: ";
    char *path_name_tail = malloc((PATH_MAX+1)*sizeof(char));
    int indent = 0; //add 4 each level deep i.e. a tab spacing
    Tree tree = NULL;
    
    Tree path_addr = NULL;
    if (path == NULL){
        PrintTreeRecur(fs->Root,indent);
       
    }
    else{
        if(strncmp(path,"/",strlen("/")) == 0){
            tree = fs->Root;
        }
        else {
            tree = fs->CWD;
        }
        Queue Q_PATH = ReturnQPath(path,tree);
        STR_Node path_part = GetQPathHead(Q_PATH);
        strcpy(path_name_tail,GetPathNameTail(Q_PATH));
        
        
        if(strcmp(path,"/") == 0 || strcmp(path,"") == 0 ){
            path_addr = fs->Root;
            
        }
        
        else if (path != NULL){
            path_addr = ReturnTreeDir(fs->Root,tree,path,err_msg,Q_PATH,path_part,FS_MODE);
        }
        
        if (path_addr != NULL){
            
            Tree tail_tree = ReturnTreeFomTail(path_addr,path_name_tail,err_msg,path,Q_PATH,FS_MODE);
            printf("%s\n",path);
            PrintTreeRecur(GetRootChildrenHead(tail_tree),indent+4);
        }
        
        QueueFree(Q_PATH);
        Q_PATH = NULL;
    }
    free(path_name_tail);
    path_name_tail = NULL;
}
/*
The function takes a path and a string, and sets the content 
of the regular file at that path to the given string. If the file already had some content, then it will be overwritten.
*/
void FsPut(Fs fs, char *path, char *content) {
    char *err_msg = "put: ";
    char *path_name_tail = malloc((PATH_MAX+1)*sizeof(char));
    Tree tree = NULL;
    
    Tree path_addr = NULL;
    
    if(strncmp(path,"/",strlen("/")) == 0){
        tree = fs->Root;
    }
    else {
        tree = fs->CWD;
    }
    Queue Q_PATH = ReturnQPath(path,tree);
    STR_Node path_part = GetQPathHead(Q_PATH);
    strcpy(path_name_tail,GetPathNameTail(Q_PATH));
    
    
    if(strcmp(path,"/") == 0 || strcmp(path,"") == 0 ){
        path_addr = fs->Root;
    }
    
    else if (path != NULL){
        path_addr = ReturnTreeDir(fs->Root,tree,path,err_msg,Q_PATH,path_part,LS_MODE);
    }
    
    if (path_addr != NULL){
        PutContent(path_addr,err_msg,path,path_name_tail,content);
    }
    
    QueueFree(Q_PATH);
    Q_PATH = NULL;
    
    free(path_name_tail);
    path_name_tail = NULL;
}
/*
The function takes a path and prints the content 
of the regular file at that path. This function roughly 
corresponds to the cat command in Linux.
*/
void FsCat(Fs fs, char *path) {
    char *err_msg = "cat: ";
    char *path_name_tail = malloc((PATH_MAX+1)*sizeof(char));
    Tree tree = NULL;
    
    Tree path_addr = NULL;
    
    if(strncmp(path,"/",strlen("/")) == 0){
        tree = fs->Root;
    }
    else {
        tree = fs->CWD;
    }
    Queue Q_PATH = ReturnQPath(path,tree);
    STR_Node path_part = GetQPathHead(Q_PATH);
    strcpy(path_name_tail,GetPathNameTail(Q_PATH));
    
    
    if(strcmp(path,"/") == 0 || strcmp(path,"") == 0 ){
        path_addr = fs->Root;
    }
    
    else if (path != NULL){
        path_addr = ReturnTreeDir(fs->Root,tree,path,err_msg,Q_PATH,path_part,LS_MODE);
    }
    
    if (path_addr != NULL){
        CatContent(path_addr,err_msg,path,path_name_tail);
    }
    
    QueueFree(Q_PATH);
    Q_PATH = NULL;
    
    free(path_name_tail);
    path_name_tail = NULL;
}
/*
The function takes a path which is expected to refer to a 
directory and deletes the directory if and only if it is empty. 
This function roughly corresponds to the rmdir command in Linux.
*/
void FsDldir(Fs fs, char *path) {
    
    char *err_msg = "dldir: failed to remove ";
    char *path_name_tail = malloc((PATH_MAX+1)*sizeof(char));
    Tree tree = NULL;
    
    Tree path_addr = NULL;
    
    if(strncmp(path,"/",strlen("/")) == 0){
        tree = fs->Root;
    }
    else {
        tree = fs->CWD;
    }
    Queue Q_PATH = ReturnQPath(path,tree);
    STR_Node path_part = GetQPathHead(Q_PATH);
    strcpy(path_name_tail,GetPathNameTail(Q_PATH));
    
    
    if(strcmp(path,"/") == 0 || strcmp(path,"") == 0 ){
        path_addr = fs->Root;
    }
    
    else if (path != NULL){
        path_addr = ReturnTreeDir(fs->Root,tree,path,err_msg,Q_PATH,path_part,LS_MODE);
    }
    
    if (path_addr != NULL){
        DlDir(path_addr,err_msg,path,path_name_tail);
    }
    
    QueueFree(Q_PATH);
    Q_PATH = NULL;
    
    free(path_name_tail);
    path_name_tail = NULL;
}
/*
The function takes a path and deletes the file at that path. 
By default, the function refuses to delete directories; 
it will only delete a directory (and all of its contents recursively) 
if recursive is true. If the path refers to a regular file, 
then the recursive argument is irrelevant. This function roughly corresponds 
to the rm command in Linux, and recursive being true corresponds to the -r option 
being used in the rm command.
*/
void FsDl(Fs fs, bool recursive, char *path) {
    
    char *err_msg = "dl: cannot remove ";
    char *path_name_tail = malloc((PATH_MAX+1)*sizeof(char));
    Tree tree = NULL;
    
    Tree path_addr = NULL;
    
    if(strncmp(path,"/",strlen("/")) == 0){
        tree = fs->Root;
    }
    else {
        tree = fs->CWD;
    }
    Queue Q_PATH = ReturnQPath(path,tree);
    STR_Node path_part = GetQPathHead(Q_PATH);
    strcpy(path_name_tail,GetPathNameTail(Q_PATH));
    
    
    if(strcmp(path,"/") == 0 || strcmp(path,"") == 0 ){
        path_addr = fs->Root;
    }
    
    else if (path != NULL){
        path_addr = ReturnTreeDir(fs->Root,tree,path,err_msg,Q_PATH,path_part,LS_MODE);
    }
    
    if (path_addr != NULL){
        Dl(path_addr,err_msg,path,path_name_tail,recursive);
    }
    
    QueueFree(Q_PATH);
    Q_PATH = NULL;
    
    free(path_name_tail);
    path_name_tail = NULL;
}

void FsCp(Fs fs, bool recursive, char *src[], char *dest) {
    // TODO
}

void FsMv(Fs fs, char *src[], char *dest) {
    // TODO
}


