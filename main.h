#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <stddef.h>
#include <stdarg.h>

//MASTER LIST OF ALL ENCODINGS BASED ON ASCII VALUE
///WARNING ENCODINGS ARE IN REVERSE ORDER IN THE LIST!!!!!!!!!!
int master_list[256];

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

int main(int argc,char ** argv);
long count(FILE*file1, FILE * binary_file);
void tree(FILE*file1, FILE*treeFile, FILE*countfile);
treeNode * add_to_stack(treeNode * stacknode, treeNode * Node,treeNode * head);
void sort_stack(treeNode * stacknode,treeNode * head);


void WT(FILE * treefile, FILE * codefile, treeNode * node,long int byte,int bytecounter);
void WriteCode(FILE * codefile, char * char_val,long int byte, int bytecounter);
void compression(FILE * fptr, FILE * treefile, FILE * codefile,FILE * compressed,long UC);
void freebin(treeNode * node);
#endif
