#ifndef SET_OP_H
#define SET_OP_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "avl.h"
#include "paragraph.h"


void INTERSECTION (AVLnode* tree1, AVLnode* tree2, AVLnode** intersection);
void UNION (AVLnode* tree1, AVLnode* tree2, AVLnode** Union);
void DIFFERENCE (AVLnode* tree1, AVLnode* tree2, AVLnode** differnece);


#endif 