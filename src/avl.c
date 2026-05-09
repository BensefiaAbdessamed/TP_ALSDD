#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"
#include "utils.h"

AVLnode* create_node (char *word) {
    AVLnode* node = malloc(sizeof(AVLnode));
    //  assigning initial values to the node
    if (node != NULL){
        node->balance_f = 0;
        node->height = 0;
        node->LC = NULL;
        node->RC = NULL;
        node->word = my_strdup(word);
    }
    return node; 
}
AVLnode* RC (AVLnode *p){
    return p->RC;
}
AVLnode* LC (AVLnode *p) {
    return p->LC;
}
int Ass_RC (AVLnode *p, AVLnode *N) {
    if (p != NULL) {p->RC = N; return 1;}
    else return 0; 
}
int Ass_LC (AVLnode *p, AVLnode *N) {
    if (p != NULL) {p->LC = N; return 1;}
    else return 0; 
}
//      returns the height of tree p
int height (AVLnode *p) {
    //  if the node is null
    if (p == NULL) return -1;
    
    // read the height
    return p->height;
}
//       return the balance factor of the tree p
int balance_f (AVLnode *p) {
    return (height(RC(p)) - height(LC(p))); 
}

//  ------- rotation algorithms -------- //
AVLnode* rotate_left (AVLnode *p) {
    AVLnode *root = p->RC;
    AVLnode *cell = root->LC;

    //  rotation performing
    root->LC = p;
    p->RC = cell;

    //  updating heights 'of trees that we changed their subtrees'
    UpdateHeight_Bf(p);
    UpdateHeight_Bf(root);

    return root;
}
AVLnode* rotate_right (AVLnode *p) {
    AVLnode *root = p->LC;
    AVLnode *cell = root->RC;

    //  rotation performing
    root->RC = p;
    p->LC = cell;

    //  updating heights 'of trees that we changed their subtrees'
    UpdateHeight_Bf(p);
    UpdateHeight_Bf(root);

    return root;
}
AVLnode *rotate_right_left(AVLnode *p) {
    p->RC = rotate_right(p->RC);
    return rotate_left(p);
}
AVLnode *rotate_left_right(AVLnode *p) {
    p->LC = rotate_left(p->LC);
    return rotate_right(p);
}

//  ------ avl management ----- //
void UpdateHeight_Bf (AVLnode *p) {
    if (p == NULL) return;
    
    int left_height = height(LC(p));
    int right_height = height(RC(p));

    int height = (left_height > right_height)? left_height: right_height;
    
    //  updating the height and the balance facotor
    p->height = height + 1;
    p->balance_f = balance_f (p);
}
//  verifies the balanace of the tree and rebalances it if needed
AVLnode* rebalance (AVLnode* p) {
    
    //  case of unbalanced tree
    if (balance_f(p) == -2) {
        if (balance_f(LC(p)) == 1) {return rotate_left_right(p);}
        else return rotate_right(p);
    }
    else if (balance_f(p) == 2) {
        if (balance_f(RC(p)) == 1) {return rotate_left(p);} 
        else return rotate_right_left(p);
    }
    
    //  balanced tree
    return p;
}

// ---------- avl core operations ---------- //
AVLnode* insert (AVLnode* root, char* word) {
    //  we reach the node 
    if (root == NULL) {return create_node(word);}
 
    // if the word is less than the root; go left
    if (strcmp(word, root->word) < 0) {root->LC = insert(LC(root), word);}
    
    // if the word is greater than the root; go right
    else if (strcmp(word, root->word) > 0) {root->RC = insert(RC(root), word);}
    
    // if the word already exists, duplicate found
    else if (strcmp(word, root->word) == 0) return root;

    // updating the height and the balance factor
    UpdateHeight_Bf(root);

    // we rotate if needed
    return rebalance(root);
}
AVLnode* search (AVLnode* root, char* word) {
    //  word doesn't exist in the tree
    if (root == NULL) return NULL;

    //  word found
    if (strcmp(root->word, word) == 0) return root;

    //  traversing the tree 
    if (strcmp(word, root->word) > 0) return search(RC(root), word);
    if (strcmp(word, root->word) < 0) return search(LC(root), word);

    //  compiler config
    return NULL;
}

