#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"

//  strdup implementation 
char *my_strdup (char *s) {
    char *copy = malloc(strlen(s) + 1);
    if (copy != NULL)
        strcpy(copy, s);
    return copy;
}

//  prints tree 
void print_tree(AVLnode *root, int space, int height) {
    if (root == NULL) return;

    space += height;

    print_tree(root->RC, space, height);

    printf("\n");
    for (int i = height; i < space; i++)
        printf(" ");
    printf("%s(h%d)\n", root->word, root->height);

    print_tree(root->LC, space, height);
}

void display(AVLnode *root) {
    print_tree(root, 0, 4);
}

//      string helpers 
char *concat(char *s1, const char *s2) {
    char *result = malloc(strlen(s1) + strlen(s2) + 2);
    strcpy(result, s1);
    strcat(result, " ");
    strcat(result, s2);
    return result;
}