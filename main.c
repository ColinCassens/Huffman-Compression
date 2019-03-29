#include "main.h"

int main(int argc,char ** argv)
{
    if(argc != 6)
    {
        return EXIT_FAILURE;
    }

    FILE * fptr = fopen(argv[1],"rb");
    if(fptr == NULL)
    {
        return EXIT_FAILURE;
    }

    //PRODUCE COUNT BINARY FILE
    FILE * count_file = fopen(argv[2],"wb");
    if(count_file ==  NULL)
    {
        return EXIT_FAILURE;
    }
    long num_characters = count(fptr, count_file);
    
    //PRODUCE TREE FILE (PRE-ORDER)
    //AS WELL AS THE CODE FILE
    FILE * treeFile = fopen(argv[3],"w+");
    FILE * codefile = fopen(argv[4],"w");
    treeNode * head = NULL;
    if(num_characters > 0)
    {
      head = tree(fptr, treeFile, codefile, num_characters);
    
      long tree_size = ftell(treeFile) / 2;
      tree_size = tree_size - 1;
      fclose(codefile);

      //create the binary lists, 1 for length 1 for code
      int * bin_list = calloc(256, sizeof(int));
      int * len_list = calloc(256, sizeof(int));
      create_list(head,bin_list,len_list);
      freetree(head);

      //Write the compressed file
      FILE * outfile = fopen(argv[5],"wb");
      huffman(fptr, outfile, bin_list, len_list, tree_size, num_characters, treeFile);

      free(bin_list);
      free(len_list);
      fclose(outfile);
    }
    else{
        FILE * outfile = fopen(argv[5],"wb");
        long zero = 0;
        long twenty4 = 24;
        fwrite(&twenty4,sizeof(long),1,outfile);
        fwrite(&zero,sizeof(long),1,outfile);
        fwrite(&zero,sizeof(long),1,outfile);
        fclose(outfile);
    }
    fclose(treeFile);
    fclose(count_file);
    fclose(fptr);
    return EXIT_SUCCESS;
}


long count(FILE * fptr,FILE * count_file)
{
    if(fptr == NULL)
    {
        return 0;
    }

    int counter_arr[256];
    for(int x = 0; x<256;x++)
    {
        counter_arr[x] = 0;
    }
    //char * char_value = calloc(1,sizeof(int));
    int ascii_value;
    long num_char = 0;

    //Runs a loop through Fptr
    while((ascii_value = fgetc(fptr)) != EOF)
    {
        counter_arr[ascii_value]++;
        num_char++;
    }

    for(int i=0; i<=255; i++)
    {
        long z = counter_arr[i];
        fwrite(&z,sizeof(long),1,count_file);
    }
    return num_char;
}



//STATUS: DONE
//TO DO: NONE
treeNode * tree(FILE * fptr, FILE * tree_file, FILE * codefile, long num_characters)
{
    //GET THE ARR OF FREQUENCIES FOR THE INPUT FILE
    fseek(fptr,0,SEEK_SET);
    int counter_arr[256];
    for(int x = 0; x<256;x++)
    {
        counter_arr[x] = 0;
    }

    //Run a loop through the fptr WORKING
    //Calculates Tree freq vals
    int ascii_value;
    int numchar = 0;
    while((ascii_value = fgetc(fptr))!= EOF)
    {
        counter_arr[ascii_value]++;
        numchar++;
    }

    treeNode * head = calloc(1,sizeof(treeNode));
    treeNode * temp = calloc(1,sizeof(treeNode));
    head->next = temp;
    int x = 0;
    int maxfreq = 0;

    //Build linked list from array In SORTED order working
    for(x = 0; x < numchar; x++)
    {
      int i = 0;
      while(i < 256 && (counter_arr[i] == 0))
      {
        i++;
      }
      if((i != 256) && (counter_arr[i] > 0))
      {
        temp->ascii_value = i;
        temp->char_val = i;
        temp->freq = counter_arr[i];
        if(temp->freq > maxfreq){
          maxfreq = temp->freq;
        }
        counter_arr[i] = 0;

        treeNode * search = head;
        while(search->next != NULL && search->next != temp)
        {
          if(search->next->freq < temp->freq)
          {
            search = search->next;
          }
          else if(search->next->freq == temp->freq)
          {
            //Test for ascii values
            if(search->next->ascii_value > temp->ascii_value)
            {
              //place it
              temp->next = search->next;
              search->next = temp;
            }
            else if(search->next->ascii_value < temp->ascii_value)
            {
              search = search->next;
            }
          }
          else if(search->next->freq > temp->freq)
          {
            temp->next = search->next;
            search->next = temp;
          }
          if(search->next == NULL && (temp->freq >= search->freq))
          {
            search->next = temp;
            temp->next = NULL;
          }
        }

        treeNode * temp2 = calloc(1,sizeof(treeNode));
        //temp->next = temp2;
        temp = temp2;
        temp->freq = 0;
        temp2 = NULL;
        free(temp2);
      }
    }

    //Get rid of extra malloc for temp node
    temp = head;
    treeNode * tempz = NULL;
    while(temp->next != NULL && temp->ascii_value != 1)
    {
      tempz = temp;
      temp = temp->next;
    }
    if(temp->ascii_value == 256)
    {
      tempz->next = NULL;
      free(temp);
    }



    //Ceate the tree by taking the first two nodes and combining, then placing in the correct location
    treeNode * stacknode = calloc(1,sizeof(treeNode));
    while(head->next->next != NULL) //The node head->next will be the root of the tree
    {
      //Add the first two items of the list to the stack
      stacknode->next = head->next;
      head->next = head->next->next->next;
      stacknode->next->next->next = NULL;

      //Create the new root node from the nodes in the stack
      treeNode * newNode = calloc(1,sizeof(treeNode));
      newNode->leftChild = stacknode->next;
      newNode->rightChild = stacknode->next->next;
      newNode->leftChild->next = NULL;
      newNode->freq = newNode->rightChild->freq + newNode->leftChild->freq;
      stacknode->next = NULL;

      //Place the root node in the linked list
      temp = head;
      while(temp->next != NULL && temp->next->freq <= newNode->freq)
      {
        temp = temp->next;
      }
      if(temp->next == NULL)
      {
        temp->next = newNode;
        newNode->next = NULL;
      }
      else if(temp->next->freq > newNode->freq)
      {
        newNode->next = temp->next;
        temp->next = newNode;
      }
      }

    int cur_path = -1;
    int depth = 0;
    Write_tree(tree_file,codefile,head->next, cur_path, depth, numchar);
    temp = head->next;
    free(head);
    free(stacknode);
    return temp;
}

void Write_tree(FILE * treefile, FILE * codefile, treeNode * node, int cur_path, int depth, int num_characters)
{
  if(node->freq > num_characters && node->leftChild->freq == num_characters)
  {
    Write_tree(treefile, codefile, node->rightChild, cur_path,depth,num_characters);
    return;
  }
  int zero = 0;
  int one = 1;
  if(node->rightChild != NULL && node->leftChild != NULL)
  {
    //WRITE TREE FILE
    fprintf(treefile,"%d",zero);
    /////////////////

    int cur_left, cur_right;
    if(cur_path == -1)
    {
      cur_left = 0;
      cur_right = 1;
    }
    else{
      cur_left = (cur_path << 1) | 0;
      cur_right = (cur_path << 1) | 1;
    }

    Write_tree(treefile, codefile, node->leftChild, cur_left, depth+1, num_characters);
    Write_tree(treefile, codefile, node->rightChild, cur_right, depth+1, num_characters);
    //////////////////


  }
  else if(node->freq > 0 && node->ascii_value < 256){
    //WRITE TREE FILE
    fprintf(treefile,"%d%c",one,node->char_val);
    /////////////////

    node->bin_code = cur_path;
    node->tree_height = depth;
    
    //PRINT CHARACTER
    fprintf(codefile,"%c:",node->char_val);
    //PRINT CODE VALUE
    int len = node->tree_height;
    while(len > 0)
    {
      int code = node->bin_code;
      int shifted = (code >> (len-1)) & 1;
      fprintf(codefile,"%d",shifted);
      len--;
    }
    fprintf(codefile,"\n");
  }
  return;
}

void freetree(treeNode * head)
{
    if(head->next != NULL){
        freetree(head->next);
    }
    if(head->leftChild != NULL){
        freetree(head->leftChild);
    }
    if(head->rightChild != NULL){
        freetree(head->rightChild);
    }
    free(head);
    return;
}