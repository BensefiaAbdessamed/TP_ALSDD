#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "avl.h"
#include "utils.h"
#include "paragraph.h"

int main()
{   
    char * paragraph = GetParagraph("test.txt");
    printf("%s", paragraph);
    return 0;
}
