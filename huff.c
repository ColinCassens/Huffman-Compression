#include "main.h"

//FILL THE TWO LISTS
//BIN_LIST: the list of the binary numbers
//LEN_LIST: the list of the length values
void create_list(treeNode * head,int * bin_list,int * len_list)
{
    if(head->rightChild != NULL && head->leftChild != NULL)
    {
        create_list(head->leftChild,bin_list,len_list);
        create_list(head->rightChild,bin_list,len_list);
    }
    else {
        bin_list[head->ascii_value] = head->bin_code;
        len_list[head->ascii_value] = head->tree_height;
    }
}

void huffman(FILE * infile, FILE * outfile, int * bin_list, int * len_list, long tree_size, long num_char, FILE * treefile)
{
    //WRITE THE FIRST THREE LONG INTS
    long num_output = 0;
    fwrite(&num_output,sizeof(long),1,outfile);
    fwrite(&tree_size,sizeof(long),1,outfile);
    fwrite(&num_char,sizeof(long),1,outfile);

    //WRITE THE TOPOLOGY OF THE HUFFMAN TREE
    //write_top(head, outfile);
    //NOT WORKING
    fseek(treefile, 0, SEEK_SET);
    int buffer = 0;
    int num = 0;
    long ascii = 0;
    int x = 1;
    while(x != EOF)
    {
        x = fscanf(treefile,"%ld",&ascii);
        //0
        if(ascii == 0)
        {
            buffer = (buffer << 1);
            num++;
        }
        //1
        else if(ascii == 1)
        {
            buffer = (buffer << 1) | 1;
            num++;
            char hold;
            x = fscanf(treefile,"%c",&hold);
            ascii = hold;
            int len = 8;
            while(len > 0)
            {
                int shifted = (ascii >> (len - 1)) & 1;
                buffer = (buffer << 1) | shifted;
                len--;
                num++;
                if(num == 8)
                {
//                    //Flip Digits NOT SURE IF NECCISSARY
//                    int k = 0;
//                    int n = 0;
//                    int s = 0;
//                    while(k < 8)
//                    {
//                        s = (buffer >> k) & 1;
//                        n = (n << 1) | s;
//                        k++;
//                    }
                    fputc(buffer,outfile);
                    num = 0;
                    buffer = 0;
                }
            }
        }     
    }


    //WRITE THE FILE IN COMPRESSED FORMAT WORKING
    fseek(infile,0,SEEK_SET);
    buffer = 0;
    num = 0;
    ascii = 0;
    while(ascii != EOF)
    {
        ascii = fgetc(infile);
        int len = len_list[ascii];
        while(len > 0)
        {
            int bin = bin_list[ascii];
            int shifted = (bin >> (len - 1)) & 1;
            buffer = (buffer << 1) | shifted;
            num++;
            len--;
            if(num == 8)
            {
                num_output += 8;
                //REVERSE THE ORDER OF THE BIT
                int l = 0;
                int new = 0;
                int shift;
                while(l < 8)
                {
                    shift = (buffer >> l) & 1;
                    new = (new << 1) | shift;
                    l++;
                }


                //WRITE THE BIT
                fputc(new,outfile);
                buffer = 0;
                num = 0;
            }
        }
    }
    if(num != 0)
    {
        num_output += num;
        num = 8 - num;
        buffer = buffer << num;

        //REVERSE ORDER
        int l1 = 0;
        int new1 = 0;
        int shift1 = 0;
        while(l1 < 8)
        {
            shift1 = (buffer >> l1) & 1;
            new1 = (new1 << 1) | shift1;
            l1++;
        }

        fputc(new1,outfile);
    }

    //Overwrites correctly, calculating value is wrong
    fseek(outfile,0,SEEK_SET);
    fwrite(&num_output,sizeof(long),1,outfile);
    return;
}
