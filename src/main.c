#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"
#include "utils.h"

int main()
{
    AVLnode* root = create_node("a");
    display(root);
    printf("Press Enter to continue...");
    getchar(); // Waits for user input

    root =  insert(root, "b");
    display(root);
    printf("Press Enter to continue...");
    getchar(); // Waits for user input

    root =  insert(root, "c");
    display(root);
    printf("Press Enter to continue...");
    getchar(); // Waits for user input

    root =  insert(root, "d");
    display(root);
    printf("Press Enter to continue...");
    getchar(); // Waits for user input

    root =  insert(root, "e");
    display(root);
    printf("finished...");
    return 0;
}
