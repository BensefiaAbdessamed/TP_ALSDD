#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "avl.h"
#include "paragraph.h"


void INTERSECTION_SENT (AVLnode* tree1, AVLnode* tree2, AVLnode** intersection){
    
    if (tree1 != NULL) {

        //  TRAVERSING THE TREE NODE BY NODE
        INTERSECTION_SENT(tree1->LC, tree2, intersection);
        INTERSECTION_SENT(tree1->RC, tree2, intersection);

        //  FIND SIMILAR SENTENCES
        char* sentence1 = tree1->word;
        AVLnode* phrase = search(tree2, sentence1); //  RETURNS A POINTER TO THE FOUND NODE
        
        
        if (phrase == NULL) {return;}
        else {*intersection = insert(*intersection, sentence1); }
    }
}

void UNION_SENT (AVLnode* tree1, AVLnode* tree2, AVLnode** Union) {
    //  inserting the 1st tree
    if (tree1 != NULL) {
        *Union = insert(*Union, tree1->word);
        UNION_SENT (tree1->LC, tree2, Union);
        UNION_SENT (tree1->RC, tree2, Union);
    }
    
    //  inserting the 2nd tree
    if (tree2 != NULL) {
        *Union = insert(*Union, tree2->word);
        UNION_SENT(tree1, tree2->LC, Union);
        UNION_SENT (tree1, tree2->RC, Union);
    }   
}

void DIFFERENCE_SENT (AVLnode* tree1, AVLnode* tree2, AVLnode** differnece) {
    
    if (tree1 != NULL) {
        //  TRAVERSE THE TREE 
        DIFFERENCE_SENT(tree1->LC, tree2, differnece);
        DIFFERENCE_SENT(tree1->RC, tree2, differnece);

        char* sentence = tree1->word;
        AVLnode* phrase = search(tree2, sentence); //  RETURNS A POINTER TO THE FOUND NODE

        if (phrase == NULL) {*differnece = insert(*differnece, sentence);}
        else return;


    }
}

void INTERSECTION_PARA (AVLnode** FILE1, AVLnode** FILE2, AVLnode** INTER_PARA){

    int i = 0;
    int index = 0;

    AVLnode* para_f1;
    AVLnode* para_f2;

    //  extract paragraphs from file1 and compare with file2
    while (FILE1[i] != NULL) {
        //  construct a string of sentences of paragraph 1
        char* full_para_f1 = calloc(1, 1);
        para_f1 = FILE1[i];
        getFullParagraph(para_f1, &full_para_f1);        //  paragaph 1

        int j = 0;
        while (FILE2[j] != NULL) {
            //  construct a string of sentences of paragraph 2
            char* full_para_f2 = calloc(1, 1);        //  @remark MUST BE EXPLAINED LATER
            para_f2 = FILE2[j];
            getFullParagraph(para_f2, &full_para_f2);        //  paragraph 2
            
            //  check if paragraphs are equal
            if (strcmp(full_para_f1, full_para_f2) == 0) {
                INTER_PARA[index++] = FILE2[j];      //   if both are equal add the paragraph to inter para
            }
            free(full_para_f2);
            
            j++;     
        }
        
        free(full_para_f1);
        i++; 
    }   

    INTER_PARA[index] = NULL;
}

void UNION_PARA (AVLnode** FILE1, AVLnode** FILE2, AVLnode** UN_PARA){
    
    int i = 0;
    int index = 0;
    char* paragraph2 = calloc(1,1);
    char* paragraph1 = calloc(1,1);

    //      addingg paragraphs from the file1
    while (FILE1[i] != NULL) {
        UN_PARA[index++] = FILE1[i++];
    }

    i = 0;
    while (FILE2[i] != NULL) {

        paragraph2 = calloc(1, 1);      //  fresh allocation for para2
        //  check if the paragraph already added in the arary
        bool exists = false;
        getFullParagraph(FILE2[i], &paragraph2);    //  got the parahraph

        for (int z = 0; z < index; z++)
        {
            paragraph1 = calloc(1, 1);      //  fresh allocation of the paragraph
            getFullParagraph(UN_PARA[z], &paragraph1);
            
            if (strcmp(paragraph1, paragraph2) == 0) {free(paragraph1); exists = true; break;}
            free(paragraph1);       //      free the paragraph 2
            
        }

        free(paragraph2);   //  free the paragraph 1
        
        //      if the paragraph isn't added before
        if (exists == false) {UN_PARA[index++] = FILE2[i];}
        i++;

    }
    
    UN_PARA[index] = NULL;      //   add the null to the end of the array
    
}

void DIFFERENCE_PARA (AVLnode** FILE1, AVLnode** FILE2, AVLnode** DIFF_PARA) {
    
    int i = 0;
    int j = 0;
    int index = 0;
    
    while (FILE1[i] != NULL)
    {
        bool exists = false;
        char *para1 = calloc(1, 1);
        getFullParagraph(FILE1[i], &para1);

        printf("%s \n", para1);
        printf("\n");
        printf("\n");
        
        j = 0;
        while (FILE2[j] != NULL)
        {
            char* para2 = calloc(1, 1);
            getFullParagraph(FILE2[j], &para2);
            printf("%s \n", para2);
            printf("\n");
            printf("\n");
            
            if (strcmp(para1, para2) == 0) {

                exists = true;
                free(para2);
                break;
            }

            j++;
            free(para2);
        }

        //  if the paragraph is not in the 2nd file

        if (exists == false) {
            DIFF_PARA[index++] = FILE1[i];
            printf("added to the diff list... \n");
        }

        i++;
        free(para1); 
    }

    DIFF_PARA[index] = NULL;
    
}


