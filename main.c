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
    printf("%ld\n",num_characters);
    //PRODUCE TREE FILE (PRE-ORDER)
    FILE * treeFile = fopen(argv[3],"wb");
    FILE * codefile = fopen(argv[4],"w");
    tree(fptr, treeFile, codefile);
    fclose(treeFile);

    //FILE * tree_top = fopen(argv[3],"r");
    //FILE * compressed = fopen(argv[5],"wb");
    //compression(fptr,tree_top,codefile,compressed,num_characters);

    //fclose(tree_top);
    //fclose(compressed);
    //fclose(codefile);
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
    //char * char_value = malloc(sizeof(int));
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

//STATUS: IN PROG
//TO DO: FINISH SORTING NUMBERS AND CREATING TREE
//DO A SERIOUS REVIEW LAST CODE SESH WAS DRUNK

void tree(FILE * fptr, FILE * tree_file, FILE * codefile)
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

    treeNode * head = malloc(sizeof(treeNode));
    treeNode * temp = malloc(sizeof(treeNode));
    head->next = temp;
    int x = 0;
    int maxfreq = 0;

    //Build linked list from array In SORTED order working
    for(x = 0; x < numchar; x++)
    {
      int i = 0;
      while((counter_arr[i] == 0) && (i < 256))
      {
        i++;
      }
      if(!((i == 256) && (counter_arr[i] <= 0)))
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

        treeNode * temp2 = malloc(sizeof(treeNode));
        //temp->next = temp2;
        temp = temp2;
      }
    }

    free(temp);
    //Get rid of extra malloc for temp node
    temp = head;
    treeNode * tempz = NULL;
    while(temp->next != NULL)
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
    treeNode * stacknode = malloc(sizeof(treeNode));
    while(head->next->next != NULL) //The node head->next will be the root of the tree
    {
      //Add the first two items of the list to the stack
      stacknode->next = head->next;
      head->next = head->next->next->next;
      stacknode->next->next->next = NULL;

      //Create the new root node from the nodes in the stack
      treeNode * newNode = malloc(sizeof(treeNode));
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
}
