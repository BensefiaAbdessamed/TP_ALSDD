#ifndef AVL_H
#define AVL_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// --------- struct definition ----------- //
typedef struct AVLnode {
    char* word;
    int height, balance_f;
    struct AVLnode* RC;
    struct AVLnode* LC;
} AVLnode;

// --------- abstract machine prototype ---------- //
AVLnode* allocate_node (char* word);
AVLnode* RC (AVLnode *P);
AVLnode* LC (AVLnode *P);
int Ass_RC (AVLnode *p, AVLnode *N);
int Ass_LC (AVLnode *p, AVLnode *N);
int height (AVLnode *p);
int balance_f (AVLnode *p);

// ---------- AVL tree functions ----------- //
void UpdateHeight_Bf (AVLnode* p);
AVLnode* rebalance (AVLnode* p);
AVLnode* insert (AVLnode *root, char *word);
AVLnode* search (AVLnode *root, char *word);

// --------- rotation algorithms -------- //
AVLnode* rotate_right (AVLnode *p);
AVLnode* rotate_left (AVLnode *p);
AVLnode* rotate_left_right (AVLnode *p);
AVLnode* rotate_right_left (AVLnode *p);


#endif // AVL_H