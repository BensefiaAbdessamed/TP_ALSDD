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

//  deconstructs paragraphs from the file and returns an array of AVL tree roots
//  must be initialized with number of paragraphss = 0 before function call
AVLnode** GetParagraphs (char* FilePath, int* num_paragraphs) {
    // opening the file on read
    FILE* fptr = fopen(FilePath, "r");
    if (fptr == NULL) {
        perror("Error opening file");
        *num_paragraphs = 0;
        return NULL;
    }

    // reading line by line
    char line[256];
    char* para = malloc(1); // Start with an empty string 
    if (para == NULL) {
        fclose(fptr);
        *num_paragraphs = 0;
        return NULL;
    }
    para[0] = '\0';

    int paragraphs_capacity = 10; // Initial capacity for the array of roots
    AVLnode** paragraph_roots = malloc(paragraphs_capacity * sizeof(AVLnode*));     // allocating the size of the array of pointers
    if (paragraph_roots == NULL) {
        free(para);
        fclose(fptr);
        *num_paragraphs = 0;
        return NULL;
    }
    *num_paragraphs = 0;
    while (fgets(line, sizeof(line), fptr)) 
    {   
        if (strlen(line) == 1)
        {
            continue;
        }
        
        // allocating more space for paragraph
        char* new_para = realloc(para, strlen(para) + strlen(line) +2);
        if (new_para == NULL) {
            // Handle realloc failure
            free(paragraph_roots);
            free(para);
            fclose(fptr);
            *num_paragraphs = 0;
            return NULL;
        }
        para = new_para;
        para = concat(para, line);      // concatinating lines into one single paragraph

        if (EndPara(line)) {
            // A paragraph has ended, process it
            lowerstring(para);              // turn characters to lower case
            delete_ponct(para);             // remove non-characters
            
            // Construct the AVL tree for the paragraph
            AVLnode* paragraph_root = GetPhrases(para);

            // Resize array of roots if necessary
            if (*num_paragraphs >= paragraphs_capacity) {
                paragraphs_capacity *= 2;
                AVLnode** new_roots = realloc(paragraph_roots, paragraphs_capacity * sizeof(AVLnode*));     //  just renaming the array of roots
                if (new_roots == NULL) {
                    // Handle realloc failure
                    free(paragraph_roots);
                    free(para);
                    fclose(fptr);
                    *num_paragraphs = 0;
                    return NULL;
                }
                paragraph_roots = new_roots;        //  the array now has new allocated size
            }

            
            // Save the root pointer into the array
            paragraph_roots[*num_paragraphs] = paragraph_root;
            (*num_paragraphs)++;

            // Reset para for the next paragraph
            para[0] = '\0';
        }

    }

    // Process the last paragraph if the file doesn't end with a paragraph marker
    if (strlen(para) > 0) {
        lowerstring(para);
        delete_ponct(para);

        AVLnode* paragraph_root = GetPhrases(para);
        if (*num_paragraphs >= paragraphs_capacity) {
            paragraphs_capacity++;
            AVLnode** new_roots = realloc(paragraph_roots, paragraphs_capacity * sizeof(AVLnode*));
            if (new_roots == NULL) {
                // Handle realloc failure
                 for (int i = 0; i < *num_paragraphs; i++) {
                    // free avl tree
                }
                free(paragraph_roots);
                free(para);
                fclose(fptr);
                *num_paragraphs = 0;
                return NULL;
            }
            paragraph_roots = new_roots;
        }
        paragraph_roots[*num_paragraphs] = paragraph_root;
        (*num_paragraphs)++;

    }
    // ensure space for the NULL terminator
    AVLnode** final = realloc(paragraph_roots, (*num_paragraphs + 1) * sizeof(AVLnode*));
    paragraph_roots = final;
    paragraph_roots[*num_paragraphs] = NULL;
    free(para);
    fclose(fptr);

    // return the array of roots
    return paragraph_roots;
}

//  deconstructs phrases from a paragraph
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

//  constructs the paragraph fron the avl tree that contains sentences 
void getFullParagraph (AVLnode* root, char** para) {
    if (root != NULL){
        //  travrse the tree of sentences
        getFullParagraph(root->LC, para);
        getFullParagraph(root->RC, para);

        //  concatenate sentences into the paragraph
        *para = concat(*para, root->word);
    }
}
