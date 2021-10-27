// Main program for testing the File System ADT

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Fs.h"

int main(void) {
    Fs fs = FsNew();
	//test alphabetical ordering
    FsMkdir(fs, "bin");
	FsMkdir(fs, "din");
	FsMkdir(fs, "ain");
	FsMkdir(fs, "cin");
	//test multi level
	FsMkdir(fs, "bin/yes");
	FsMkdir(fs, "bin/yes/omg");
	FsMkfile(fs, "bin/yes/omg/yes.txt");
	FsMkfile(fs, "cin/pwd");
	FsMkfile(fs, "hello.txt");
	FsMkfile(fs, "world.txt");
	FsMkdir(fs, "home");
	FsMkdir(fs, "home/jas");
	FsMkfile(fs, "home/jas/todo.txt");
	FsMkfile(fs, "home/jas/mail.txt");
	FsTree(fs, "/home/jas");
	printf("---\n"); // marker to separate output
	FsCd(fs, "/home");
	FsLs(fs,"/home");
	FsTree(fs, NULL);
    FsFree(fs);
}

