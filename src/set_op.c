#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "avl.h"
#include "paragraph.h"


void INTERSECTION (AVLnode* tree1, AVLnode* tree2, AVLnode** intersection){
    
    if (tree1 != NULL) {

        //  TRAVERSING THE TREE NODE BY NODE
        INTERSECTION(tree1->LC, tree2, intersection);
        INTERSECTION(tree1->RC, tree2, intersection);

        //  FIND SIMILAR SENTENCES
        char* sentence1 = tree1->word;
        AVLnode* phrase = search(tree2, sentence1); //  RETURNS A POINTER TO THE FOUND NODE
        
        
        if (phrase == NULL) {return;}
        else {*intersection = insert(*intersection, sentence1); }
    }
}

void UNION (AVLnode* tree1, AVLnode* tree2, AVLnode** Union) {
    //  inserting the 1st tree
    if (tree1 != NULL) {
        *Union = insert(*Union, tree1->word);
        UNION (tree1->LC, tree2, Union);
        UNION (tree1->RC, tree2, Union);
    }
    
    //  inserting the 2nd tree
    if (tree2 != NULL) {
        *Union = insert(*Union, tree2->word);
        UNION (tree1, tree2->LC, Union);
        UNION (tree1, tree2->RC, Union);
    }   
}

void DIFFERENCE (AVLnode* tree1, AVLnode* tree2, AVLnode** differnece) {
    if (tree1 != NULL) {
        //  TRAVERSE THE TREE 
        DIFFERENCE(tree1->LC, tree2, differnece);
        DIFFERENCE(tree1->RC, tree2, differnece);

        char* sentence = tree1->word;
        AVLnode* phrase = search(tree2, sentence); //  RETURNS A POINTER TO THE FOUND NODE

        if (phrase == NULL) {*differnece = insert(*differnece, sentence);}
        else return;


    }
}