#ifndef SET_OP_H
#define SET_OP_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "avl.h"
#include "paragraph.h"


void INTERSECTION_SENT (AVLnode* tree1, AVLnode* tree2, AVLnode** intersection);
void UNION_SENT (AVLnode* tree1, AVLnode* tree2, AVLnode** Union);
void DIFFERENCE_SENT (AVLnode* tree1, AVLnode* tree2, AVLnode** differnece);

void INTERSECTION_PARA (AVLnode** FILE1, AVLnode** FILE2, AVLnode** INTER_PARA);
void UNION_PARA (AVLnode** FILE1, AVLnode** FILE2, AVLnode** UN_PARA);
void DIFFERENCE_PARA (AVLnode** FILE1, AVLnode** FILE2, AVLnode** DIFF_PARA);

#endif 