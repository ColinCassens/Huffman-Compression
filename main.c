//#ifdef TEST_MAIN
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

    //Build linked list from array working
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
        treeNode * temp2 = malloc(sizeof(treeNode));
        temp->next = temp2;
        temp = temp2;
      }
      else{
        free(temp);
      }
    }

    //Remove NULL node from list. Can be used as stack head but not needed
    temp = head;
    while(temp->next->next != NULL)
    {
      temp = temp->next;
    }
    temp->next = NULL;

    //Sort the code file based on count and ascii value
    //Do this by getting the nodes with that frequency and putting them in the correct order. Then add them to the array. Stack may be useful for this.
    Node * temp1 = head->next;
    int max_ascii = 0;
    while(maxfreq != 0)
    {
      if(temp1->freq != maxfreq)
      {
        temp1 = temp1->next;
      }
      if(temp1->freq == maxfreq)
      {
        //At least one node of this freq exists add the ndoe to stack and sort and add to front of list
      }
      else if(temp1->next == NULL)
      {
          //A Node with this frequency does not exist FUCK IT move on to the next freq
      }

    }

    //Ceate the tree by taking the first two nodes and combining, then placing in the correct location
    treeNode * stacknode = malloc(sizeof(treeNode));
    while(head->next != NULL)
    {
      //Add the first two items of the list to the stack
      stack->next = head->next;
      head->next = head->next->next->next;
      stack->next->next->next = NULL;
      treeNode * newnode = malloc(sizeof(treeNode));
      newNode->rightChild = stacknode->next;
      newNode->leftChild = stacknode->next->next;
      newNode->rightChild->next = NULL;
      newNode->freq = newNode->rightChild->freq + newNode->leftChild->freq;



      temp1 = head->next;
      //Find the placement for this new node and put it there
      while(newNode->freq > temp1->freq)
      {
        temp1 = temp1->next;
      }
      if(newNode->freq == temp1->freq)
      {
        //Compare ascii values
      }
      else{
        //Place the new node here
        newNode->next = temp1->next;
        temp1->next = newNode->next;
      }

    }
}

treeNode * CN(int val, int freq)
{
    treeNode * temp = malloc(sizeof(treeNode));
    temp -> ascii_value = val;
    temp -> char_val = val;
    temp -> freq = freq;
    temp -> next = NULL;
    temp -> dist = 0;
    temp ->loc = 0;
    temp -> rightChild = NULL;
    temp -> leftChild = NULL;
    return temp;
}

void WT(FILE * treefile,FILE * codefile, treeNode * node,long int byte,int bitcounter)
{
    int zero = 0;
    int one = 1;
    int mask = 0x1;
    long int tempbyte = 0x0;
    if(node -> leftChild != NULL)
    {
        fprintf(treefile,"%d",zero);
        tempbyte = byte<<1;
        node->leftChild->loc = node->loc * 10;
        WT(treefile,codefile,node->leftChild,tempbyte,bitcounter+1);
    }
    if(node->rightChild != NULL)
    {
        tempbyte = (byte<<1) | mask;
        node->rightChild->loc = node->loc * 10 + 1;
        WT(treefile,codefile,node->rightChild,tempbyte,bitcounter+1);
    }
    if(node->rightChild == NULL && node->leftChild == NULL)
    {
        fprintf(treefile,"%d",one);
        fprintf(treefile,"%c",node->char_val);
        WriteCode(codefile,&node->char_val,node->loc,bitcounter);
    }
}

void WriteCode(FILE * codefile, char * char_val,long int byte,int bitcounter)
{
    int ascii_value = *char_val;
    byte = byte * 10 + 9;
    long int reverse = 0;
    while(byte !=  0)
    {
        reverse = reverse * 10;
        reverse = reverse + byte%10;
        byte = byte/10;
    }
    reverse = reverse / 10;
    master_list[ascii_value] = reverse;
    //nt * x = malloc(sizeof(int));
    // *x = *char_val;
    // if(*x != NULL)
    // {
    //     fprintf(codefile,"%s:",char_val);
    //     while(reverse%10 != 9 && reverse > 0)
    //     {
    //         fprintf(codefile,"%ld",reverse%10);
    //         reverse = reverse / 10;
    //     }
    //     fprintf(codefile,"\n");
    // }

}


void freebin(treeNode * node)
{
    if(node->rightChild != NULL)
    {
        freebin(node->rightChild);
    }
    if(node->leftChild != NULL)
    {
        freebin(node->leftChild);
    }
    free(node);
}
//#endif
