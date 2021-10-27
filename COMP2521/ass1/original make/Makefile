# COMP2521 Assignment 1

# !!! DO NOT MODIFY THIS FILE !!!

CC = gcc
CFLAGS = -Wall -Werror -g

all: testFs testFsColored

testFs: testFs.c Fs.c utility.c utility.h listFile.c
	$(CC) $(CFLAGS) -o testFs testFs.c Fs.c utility.c listFile.c

testFsColored: testFs.c Fs.c utility.c utility.h listFile.c
	$(CC) $(CFLAGS) -DCOLORED -o testFsColored testFs.c Fs.c utility.c listFile.c

clean:
	rm -f testFs testFsColored

