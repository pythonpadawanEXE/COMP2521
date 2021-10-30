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
    //GetNewTree(char *canonical_path,char* curr_path,char *parent_path,FileType tree_type,Tree parent_tree)
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

void FsFree(Fs fs) {
    // TODO
    
    if(fs->Root != NULL){
        //will path variabls in tree structs need to be mallocced and freed?
        Tree tree = GetRootChildrenHead(fs->Root);
        FsFreeTree(tree);
        FreeRoot(fs->Root);
        fs->Root = NULL;
        free(fs);
        fs = NULL;
    }
     
}
//want to make dirs inserted into list alphabetically
void FsMkdir(Fs fs, char *path) {
    // TODO
        
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
        //make new tree
        //getNewRootTree(char *canonical_path,char* curr_path,char *parent_path,FileType tree_type)
        char *parent_path = GetParentPath(path_addr);
        char *canonical_path = malloc((PATH_MAX+1)*sizeof(char));
        char *safe_canonical_path = malloc((PATH_MAX+1)*sizeof(char));
        assert(canonical_path != NULL);
        assert(safe_canonical_path != NULL);

        
        
        strcpy(canonical_path,parent_path);
        strcat(canonical_path,path_name_tail);
        strcpy(safe_canonical_path,canonical_path);
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
    // TODO
        
    //Return Path check for errors and handle finding path
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

    Tree path_addr = ReturnTreeDir(fs->Root,tree,path,err_msg,Q_PATH,path_part,MK_MODE);

    //make new subtree at path_addr as a directory in children_head where path_addr is parent of children_head
    if(path_addr != NULL){
        //make new tree
        //getNewRootTree(char *canonical_path,char* curr_path,char *parent_path,FileType tree_type)
        char *parent_path = GetParentPath(path_addr);
        char *canonical_path = malloc((PATH_MAX+1)*sizeof(char));
        char *safe_canonical_path = malloc((PATH_MAX+1)*sizeof(char));
        assert(canonical_path != NULL);
        assert(safe_canonical_path != NULL);

        
        
        strcpy(canonical_path,parent_path);
        strcat(canonical_path,path_name_tail);
        strcpy(safe_canonical_path,canonical_path);
        Tree NewTree = GetNewTree(safe_canonical_path,path_name_tail,parent_path,REGULAR_FILE,path_addr);
        //add new tree in alphabetical order inside path_address children if it matches a name at the same level free tree and print error of file exists
        AddTree(path_addr,NewTree,err_msg,path,path_name_tail);
        free(canonical_path);
        canonical_path = NULL;
    }
    else {
        //is this needed?????
        free(path_name_tail);
        path_name_tail = NULL;
    }
    
    QueueFree(Q_PATH);
    Q_PATH = NULL;
}

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
            //path_addr = s->CWD
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
    //TODO
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

void FsPwd(Fs fs) {
    // TODO
    char *str = GetCanonicalPath(fs->CWD);
    if(strcmp(str,ROOT_PATH) == 0){
        printf("/\n");
    }
    else{
        printf("%s\n",str+strlen(ROOT_PATH));
    }
}

void FsTree(Fs fs, char *path) {
    // TODO
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
            //path_addr = s->CWD
        }
        
        else if (path != NULL){
            path_addr = ReturnTreeDir(fs->Root,tree,path,err_msg,Q_PATH,path_part,FS_MODE);
        }
        
        if (path_addr != NULL){
            
            Tree tail_tree = ReturnTreeFomTail(path_addr,path_name_tail,err_msg,path,Q_PATH,FS_MODE);
            
            PrintTreeRecur(tail_tree,indent);
        }
        
        QueueFree(Q_PATH);
        Q_PATH = NULL;
    }
    free(path_name_tail);
    path_name_tail = NULL;
}

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

void FsCat(Fs fs, char *path) {
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
        CatContent(path_addr,err_msg,path,path_name_tail);
    }
    
    QueueFree(Q_PATH);
    Q_PATH = NULL;
    
    free(path_name_tail);
    path_name_tail = NULL;
}

void FsDldir(Fs fs, char *path) {
    // TODO
}

void FsDl(Fs fs, bool recursive, char *path) {
    // TODO
}

void FsCp(Fs fs, bool recursive, char *src[], char *dest) {
    // TODO
}

void FsMv(Fs fs, char *src[], char *dest) {
    // TODO
}


