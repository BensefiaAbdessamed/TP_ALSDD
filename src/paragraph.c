#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "avl.h"
#include "utils.h"
#include "paragraph.h"
#include <string.h>
#include <ctype.h>

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

//  detects letters
bool is_letter (char c){
    if ((c >= 'a' && c <= 'z') || c == ' ' || c == '.') return true;
    else return false;
}

//  turn string to lowercase
void lowerstring (char* str) {
    for (int i = 0; str[i]; i++)
    str[i] = tolower(str[i]);
}

//  delete ponctuation
void delete_ponct (char* str) {
    if (str == NULL) return;

    int i = 0, j = 0;
    while (str[i]) {
        if (is_letter(str[i]))
            str[j++] = str[i];
        i++;
    } 
    str[j] = '\0';
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
        lowerstring(para);              //  turn characters to lower case
        delete_ponct(para);             //  remove non-characters
        if (EndPara(line)) {
            

        }; 
    }

    //  return the paragraph
    return para;
}

AVLnode* GetPhrases (char* paragraph) {
    if (paragraph == NULL) return NULL;

    AVLnode* paragraph1 = NULL;
    size_t len = strlen(paragraph);
    size_t start = 0;

    for (size_t i = 0; i <= len; i++) {
        if (paragraph[i] == '.' || paragraph[i] == '\0') {
            size_t end = i;

            while (start < end && isspace((unsigned char)paragraph[start])) start++;
            while (end > start && isspace((unsigned char)paragraph[end - 1])) end--;

            if (end > start) {
                size_t sentence_len = end - start;
                char* sentence = malloc(sentence_len + 1);
                if (sentence != NULL) {
                    memcpy(sentence, paragraph + start, sentence_len);
                    sentence[sentence_len] = '\0';
                    paragraph1 = insert(paragraph1, sentence);
                    free(sentence);
                }
            }

            start = i + 1;
        }
    }

    return paragraph1;
}

