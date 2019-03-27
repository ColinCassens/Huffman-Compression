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

  //FOR CREATING THE CODE FILE
  int bin_code;
  int tree_height; //TELLS US THE TOTAL NUMBER OF CHARACTERS IT SHOULD BE
} treeNode;

//WORKING
int main(int argc,char ** argv);
long count(FILE*file1, FILE * binary_file);
treeNode * tree(FILE*file1, FILE*treeFile, FILE*countfile, long num_characters);
//IN PROG.
//SOMETHING WRONG WITH BINARY
void Write_tree(FILE * treefile, FILE * codefile, treeNode * node, int cur_path, int depth, long num_characters);
void create_list(treeNode * head,int * bin_list,int * len_list);
void huffman(FILE * infile, FILE * outfile, int * bin_list, int * len_list, long tree, long num_char, FILE * treefile);
void write_top(treeNode * node, FILE * outfile);

#endif
