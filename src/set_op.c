#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "avl.h"
#include "paragraph.h"

// Helper to traverse a tree and perform an action
static void traverse_and_intersect(AVLnode* t1, AVLnode* t2, AVLnode** res) {
    //  if we reach a null
    if (t1 == NULL) return;

    //  traversing the tree i.e traversing each sentence in the paragraph   1
    traverse_and_intersect(t1->LC, t2, res);
    traverse_and_intersect(t1->RC, t2, res);
    
    //  search for the word if it exists in the paragraph 2
    if (search(t2, t1->word)) {
        *res = insert(*res, t1->word);      //  if yes insert it into the result AVL tree
    }
}
static void traverse_and_insert(AVLnode* src, AVLnode** dest) {
    //  skip if the we reach a null
    if (src == NULL) return;

    //  insert the sentence we reached 
    *dest = insert(*dest, src->word);

    //  then traverse all the tree
    traverse_and_insert(src->LC, dest);
    traverse_and_insert(src->RC, dest);
}
static void traverse_and_diff(AVLnode* t1, AVLnode* t2, AVLnode** res) {
    //  if we reach a null 
    if (t1 == NULL) return;

    //  traverse the tree 1
    traverse_and_diff(t1->LC, t2, res);
    traverse_and_diff(t1->RC, t2, res);
    
    //  the word doesn't exist in the 2nd paragraph 
    if (!search(t2, t1->word)) {
        *res = insert(*res, t1->word);      //  insert once it's not found
    }
}

void INTERSECTION_SENT (AVLnode* tree1, AVLnode* tree2, AVLnode** intersection) {
    traverse_and_intersect(tree1, tree2, intersection);
}
void UNION_SENT (AVLnode* tree1, AVLnode* tree2, AVLnode** Union) {
    //  insert the tree 1 and tree 2
    traverse_and_insert(tree1, Union);
    traverse_and_insert(tree2, Union);
}
void DIFFERENCE_SENT (AVLnode* tree1, AVLnode* tree2, AVLnode** differnece) {
    //  calls the static trav and diff funcion
    traverse_and_diff(tree1, tree2, differnece);
}

//   helper to compare two AVL trees (paragraphs)
static bool are_paras_equal(AVLnode* p1, AVLnode* p2) {
    char *s1 = calloc(1, 1);
    char *s2 = calloc(1, 1);

    //  extracting paragraphs from the trees
    getFullParagraph(p1, &s1);          //      paragraph 1
    getFullParagraph(p2, &s2);          //      paragraph 2
    
    bool equal = (strcmp(s1, s2) == 0);
    free(s1); free(s2);
    return equal;
}

void INTERSECTION_PARA (AVLnode** FILE1, AVLnode** FILE2, AVLnode** INTER_PARA) {
    int index = 0;
    for (int i = 0; FILE1[i]; i++) {                    //  traverse each paragraph in file1
        for (int j = 0; FILE2[j]; j++) {                //  traverse each pragraph in file2
            if (are_paras_equal(FILE1[i], FILE2[j])) {  //  check if equal
                INTER_PARA[index++] = FILE1[i];         //  insert into inter para if equal
                break;
            }
        }
    }
    INTER_PARA[index] = NULL;       //  add null to indicate the end of the file 
}
void UNION_PARA (AVLnode** FILE1, AVLnode** FILE2, AVLnode** UN_PARA) {
    int index = 0;
    for (int i = 0; FILE1[i]; i++) UN_PARA[index++] = FILE1[i];     //  insert paragraphs of file 1 all
    
    for (int i = 0; FILE2[i]; i++) {
        bool exists = false;
        
        //  check if the paragraph already added
        for (int j = 0; j < index; j++) {
            if (are_paras_equal(FILE2[i], UN_PARA[j])) {
                exists = true;
                break;
            }
        }
        
        //  add if the para doesn't exists
        if (!exists) UN_PARA[index++] = FILE2[i];
    }
    UN_PARA[index] = NULL;      //  add null at the end of file
}
void DIFFERENCE_PARA (AVLnode** FILE1, AVLnode** FILE2, AVLnode** DIFF_PARA) {
    int index = 0;
    for (int i = 0; FILE1[i]; i++) {
        bool exists = false;
        for (int j = 0; FILE2[j]; j++) {
            if (are_paras_equal(FILE1[i], FILE2[j])) {
                exists = true;
                break;
            }
        }
        if (!exists) DIFF_PARA[index++] = FILE1[i];     //  insert if the paragraph hasn't been repeated in file 2
    }
    DIFF_PARA[index] = NULL;
}



