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
    //  the file path
    char* file_path = "test.txt";

    // Get the paragraphs as an array of AVL tree roots
    AVLnode** paragraphs = GetParagraph(file_path, &num_paragraphs);
    AVLnode* I = NULL;
    AVLnode* D = NULL;
    AVLnode* U = NULL;

    printf("INTERSECTION: \n");
    INTERSECTION(paragraphs[0], paragraphs[1], &I);
    if (I == NULL) printf("null \n");
    display(I);
    free(I);
    
    printf("UNION: \n");
    UNION(paragraphs[0], paragraphs[1], &U);
    if (U == NULL) printf("null \n");
    display(U);
    free(U);
    
    printf("DIFFERENCE: \n");
    DIFFERENCE(paragraphs[0], paragraphs[1], &D);
    if (D == NULL) printf("null \n");
    display(D);
    free(D);

    return 0;
}
