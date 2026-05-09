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
char *concat(char *s1, char *s2) {
    if (!s2) return s1;
    size_t s1_len = s1 ? strlen(s1) : 0;
    size_t s2_len = strlen(s2);
    // +1 for space (if s1 exists), +1 for null terminator
    size_t need_space = (s1_len > 0) ? 1 : 0;
    char* result = realloc(s1, s1_len + need_space + s2_len + 1);
    if (!result) return NULL;

    if (s1_len > 0) {
        strcat(result, " ");
        strcat(result, s2);
    } else {
        strcpy(result, s2);
    }
    return result;
}