#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "avl.h"
#include "utils.h"
#include "paragraph.h"
#include "set_op.h"

int main()
{   
    int num_paragraphs = 0;
    int num_paragraphs2 = 0;
    //  the file path
    char* file_path = "test.txt";
    char* file_path2 = "the_silence_between_stars.txt";

    // Get the paragraphs as an array of AVL tree roots
    AVLnode** paragraphs = GetParagraphs(file_path, &num_paragraphs);
    AVLnode** paragraphs2 = GetParagraphs(file_path2, &num_paragraphs2);

    // display(paragraphs[0]);
    // printf("\n");
    // printf("\n");
    // display(paragraphs[1]);
    // printf("\n");
    // printf("\n");
    // display(paragraphs[2]);
    // printf("\n");
    // printf("\n");



    AVLnode** I = calloc(num_paragraphs + 1, sizeof(AVLnode*));
    printf("HELLO\n");
    INTERSECTION_PARA(paragraphs, paragraphs2, I);
    char* para = calloc(1, 1);
    int i = 0;

    while (I[i] != NULL) {
        para = calloc(1, 1);
        getFullParagraph(I[i++], &para);
        printf("%s \n", para);
        printf("\n");
        printf("\n");
    }
    // display(I[0]);

    // char* sentence = calloc(1,1);
    // getFullParagraph(I[0], &sentence);
    // printf("%s \n", sentence);
    // printf("\n");


    // printf("INTERSECTION: \n");
    // INTERSECTION(paragraphs[0], paragraphs[1], &I);
    // if (I == NULL) printf("null \n");
    // display(I);
    // free(I);
    
    // printf("UNION: \n");
    // UNION(paragraphs[0], paragraphs[1], &U);
    // if (U == NULL) printf("null \n");
    // display(U);
    // free(U);
    
    // printf("DIFFERENCE: \n");
    // DIFFERENCE(paragraphs[0], paragraphs[1], &D);
    // if (D == NULL) printf("null \n");
    // display(D);
    // free(D);

    return 0;
}
