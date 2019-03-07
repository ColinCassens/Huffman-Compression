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

    //PRODUCE TREE FILE (PRE-ORDER)
    FILE * treeFile = fopen(argv[3],"w");
    FILE * codefile = fopen(argv[4],"w");
    tree(fptr, treeFile, codefile);
    fclose(treeFile);

    FILE * tree_top = fopen(argv[3],"r");
    FILE * compressed = fopen(argv[5],"wb");
    compression(fptr,tree_top,codefile,compressed,num_characters);

    fclose(tree_top);
    fclose(compressed);
    fclose(codefile);
    fclose(count_file);
    fclose(fptr);

    return EXIT_SUCCESS;
}

void compression(FILE * fptr, FILE * treefile, FILE * codefile,FILE * compressed,long UC)
{
    long z = 0;
    fseek(treefile,0,SEEK_END);
    int numbytes = ftell(treefile) / 2;
    long x = numbytes - 1;
    fseek(compressed,0,SEEK_SET);

    //WRITE HEADER LONGS WORKING
    fwrite(&z,sizeof(long),1,compressed);
    fwrite(&x,sizeof(long),1,compressed);
    fwrite(&UC,sizeof(long),1,compressed);

    //Reset position of fptr to the beggining
    fseek(fptr,0,SEEK_SET);
    fseek(treefile,0,SEEK_SET);
    char onebit;
    // char c;
    // int mybyte = 0x0;
    // int character;
    // int char_counter;
    // int bit_count;
    // int bit = 0x0;
    // int mask = 0x1;
    // int currentbit = 1;
    // int currentbyte = 0x0;
    // int bytecounter;

    //CANNOT GET THE HEADER TO PRINT IN THE CORRRECT FORMAT
    while((onebit = fgetc(treefile)) != EOF)
    {   
        // mybyte = 0x0;
        // for(bit_count = 7; bit_count>=0; bit_count--)
        // {
        //     bit = ((onebit >> bit_count) | 0x1);
        //     mybyte = ((mybyte << 1) | bit);
        // }
        fwrite(&onebit,sizeof(char),1,compressed);
            // if(bit == 1)
            // {
            //     mask = 0x1;
            //     if(currentbit > 8)
            //     {
            //         fwrite(&currentbyte,sizeof(char),1,compressed);
            //         currentbyte = 0x0;
            //         currentbit = 1;
            //     }
            //     currentbyte |= mask << (8 - currentbit);
            //     currentbit++;

            //     c = fgetc(treefile);
            //     character = c;
            //     for(char_counter = 7; char_counter >= 0; char_counter--)
            //     {
            //         if(currentbit > 8)
            //         {
            //             fwrite(&currentbyte,sizeof(char),1,compressed);
            //             currentbyte = 0x0;
            //             currentbit = 1;
            //         }
            //         mask = (character >> char_counter) | 0x1;
            //         if(currentbit <= 8)
            //         {
            //             currentbyte |= mask << (8 - currentbit);
            //             currentbit++;
            //         }
                    
            //     }
            // }
            // else
            // {
            //     currentbit++;
            // }
        // }
    }

    // Write the rest of the compressed file to the output
    



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
    while((ascii_value = fgetc(fptr))!= EOF)
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
    int ascii_value;
    int numchar = 0;
    while((ascii_value = fgetc(fptr))!= EOF)
    {
        counter_arr[ascii_value]++;
        numchar++;
    }

    //GET min index
    int min = 1;
    int min_index = 0;
    int control = 1;
    int y = 0;
    if(numchar != 0)
    {
        while(control == 1)
        {
            y = 0;
            while(control == 1 && y < 256)
            {
                if(counter_arr[y] == min)
                {
                    min = counter_arr[y];
                    min_index = y;
                    control = 0;
                }
                else
                {
                    y++;
                }
            }
            if(control != 0)
            {
                min++;
            }
        }

        int max = 0;
        for(int g = 0; g<256;g++)
        {
            if(counter_arr[g] > max)
            {
                max = counter_arr[g];
            }
        }

        //treeNode * node;
        treeNode * head = CN(0,0);
        head->ascii_value = min_index;
        head->char_val = min_index;
        head->freq = min;
        head->dist = 0;
        head->next = NULL;

        //CREATES LIST
        int counter2 = 1;
        int num_nodes = 1;
        int k = 0;
        while(max >= counter2)
        {
            for(k = 0; k < 256; k++)
            {
                if(counter_arr[k] == counter2 && k != min_index)
                {
                    treeNode * node = NULL;
                    node = CN(k,counter2);
                    node->next = head;
                    head = node;
                    num_nodes++;
                }
            }
            counter2++;
        }

        //CREATE TREE
        treeNode * newNode;
        treeNode * tempNode;

        while(num_nodes != 2)
        {
            tempNode = CN(0,0);
            newNode = head; 
            while(newNode->next->next->next != NULL)
                {
                    newNode = newNode->next;
                }
            tempNode->rightChild = newNode->next;
            tempNode->leftChild = newNode->next->next;
            tempNode->freq = tempNode -> rightChild->freq + tempNode->leftChild->freq;
            tempNode->ascii_value = tempNode -> rightChild->ascii_value + tempNode->leftChild->ascii_value;
            newNode->next = NULL;
            newNode = head;
            if(tempNode->rightChild->next == tempNode->leftChild)
            {
                tempNode->rightChild->next = NULL;
            }

            //Positioning the temp node
            while(newNode -> freq > tempNode->freq)
            {
                newNode = newNode->next;
            }
            if(newNode -> freq < tempNode->freq)
            {
                tempNode->next = newNode;
                if(newNode == head)
                {
                    head = tempNode;
                }
            }
            else if(newNode->freq == tempNode->freq)
            {
                if(newNode->ascii_value < tempNode->ascii_value)
                {
                    tempNode->next = newNode;
                    if(newNode == head)
                    {
                        head = tempNode;
                    }
                }
                if(newNode->ascii_value > tempNode->ascii_value)
                {
                    tempNode->next = newNode->next;
                }
            }

            newNode = head;
            if(head != tempNode)
            {
                while(newNode->next != tempNode->next)
                {
                    newNode = newNode->next;
                }
                newNode->next = tempNode;        
            }

            num_nodes--;
        }
        //Finish off the last two nodes of the tree
        if(num_nodes == 2)
        {
            tempNode = CN(0,0);
            if(head->freq > head->next->freq)
            {
                tempNode->rightChild = head;
                tempNode->leftChild = head->next;
                tempNode->rightChild->next = NULL;
                tempNode->freq = tempNode->rightChild->freq + tempNode->leftChild->freq;
            }
            else if(head->freq < head->next->freq)
            {
                tempNode->rightChild = head->next;
                tempNode->leftChild = head;
                tempNode->rightChild->next = NULL;
                tempNode->freq = tempNode->rightChild->freq + tempNode->leftChild->freq;
                
            }
            else if(head->freq == head->next->freq)
            {
                if(head->ascii_value > head->next->ascii_value)
                {
                    tempNode->rightChild = head;
                    tempNode->leftChild = head->next;
                    tempNode->rightChild->next = NULL;
                    tempNode->freq = tempNode->rightChild->freq + tempNode->leftChild->freq;
                }
                else
                {
                    tempNode->rightChild = head->next;
                    tempNode->leftChild = head;
                    tempNode->rightChild->next = NULL;
                    tempNode->freq = tempNode->rightChild->freq + tempNode->leftChild->freq;
                }
            }
            head = tempNode;
            head->loc = 9;
            
        }
        long int byte = 0x0;
        WT(tree_file,codefile,head,byte,0);

        freebin(head);
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
    int x = *char_val;
    if(&x != NULL)
    {
        fprintf(codefile,"%s:",char_val);
        while(reverse%10 != 9 && reverse > 0)
        {
            fprintf(codefile,"%ld",reverse%10);
            reverse = reverse / 10;
        }
        fprintf(codefile,"\n");
    }

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
