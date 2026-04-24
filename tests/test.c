#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"

int main(int argc, char const *argv[])
{
    allocate_node("hello");
    allocate_node("world");
    allocate_node("name");
    return 0;
}
