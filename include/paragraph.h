#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "avl.h"
#include "utils.h"
#include <string.h>

/*
1-  reading files
2-  removing ponctuation 
3-  turning words into lower case
4-  deconstructing words  
5-  inserting words into a tree
6-  detecting the end of a paragraph 
*/
AVLnode** GetParagraphs (char* FilePath, int* num_paragraphs);
AVLnode* GetPhrases (char* paragraph);
bool EndPara (char* string);
void delete_ponct (char* c);
bool is_letter (char c);
void lowerstring (char* str);
void delete_ponct (char* str);
void getFullParagraph (AVLnode* root, char** para);