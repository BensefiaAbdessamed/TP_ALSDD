#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "avl.h"
#include "utils.h"
#include "paragraph.h"
#include <string.h>

//  detects the end of the paragraph
bool EndPara (char* string){
    
    int i = 0;
    bool end = false; 

    //   traversing the whole string
    while (string[i+2] != '\0') i++;
    
    //  checking the last letter
    if ((string[i] == '.' && string[i+1] == '\n' ) ||
    (string[i-1] == '.' && string[i] == ' ' && string[i+1] == '\n')) end = true;

    //  return the state of the paragraph
    return end;
}

//  deconstructs a paragraph from the  file
char* GetParagraph (char* FilePath) {
    //  opening the file on read
    FILE* fptr = fopen(FilePath, "r");

    //  reading line by line
    char line[256];
    char* para = malloc(sizeof(line));

    while (fgets(line, sizeof(line), fptr)) 
    {   
        para = realloc(para, strlen(line) + strlen(para) + 2);  //  allocating more space for paragraph
        para = concat(para, line);      //  concatinating lines into one sigle paragraph
        if (EndPara(line)) break;       //  check if we reach the end of the paragraph
    }

    //  return the paragraph
    return para;
}