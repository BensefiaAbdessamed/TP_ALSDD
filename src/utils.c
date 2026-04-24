#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *my_strdup (char *s) {
    char *copy = malloc(strlen(s) + 1);
    if (copy != NULL)
        strcpy(copy, s);
    return copy;
}