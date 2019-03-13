#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <stddef.h>
#include <stdarg.h>


typedef struct tNode
{
  struct tNode * next;
	struct tNode * rightChild;
	struct tNode * leftChild;
	int ascii_value;
  char char_val;
  int freq;
  int loc;
  int dist;
} treeNode;

//WORKING
int main(int argc,char ** argv);
long count(FILE*file1, FILE * binary_file);
void tree(FILE*file1, FILE*treeFile, FILE*countfile);
//IN PROG.
void Write_tree(FILE * treefile, FILE * codefile, treeNode * node);
//OBSOLETE
void WriteCode(FILE * codefile, char * char_val,long int byte, int bytecounter);
void compression(FILE * fptr, FILE * treefile, FILE * codefile,FILE * compressed,long UC);
void freebin(treeNode * node);
#endif
