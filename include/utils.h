#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"

char* my_strdup (char *s);
void print_tree(AVLnode *root, int space, int height);
void display(AVLnode *root);
char *concat(char *s1, const char *s2);

#endif // UTILS_H