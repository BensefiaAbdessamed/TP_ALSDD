#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "avl.h"
#include "paragraph.h"
#include "set_op.h"
#include "utils.h"

// ANSI Color Codes
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define CYAN        "\033[36m"
#define BOLD_CYAN   "\033[1;36m"
#define WHITE       "\033[37m"
#define BOLD_WHITE  "\033[1;37m"
#define YELLOW      "\033[33m"
#define GREEN       "\033[32m"
#define RED         "\033[31m"
#define GRAY        "\033[90m"

// UI Helpers
void clear_screen();
void print_separator(char* title);
void centered_text(char* text, int width);
void splash_screen();
void loading_animation(char* msg);

// Main Logic Flows
void run_app();
void main_menu();
void file_loading_flow();
void operation_flow();
void view_files_summary();
char post_operation_menu();

// Helper Structures
typedef struct LoadedFile {
    char name[256];
    AVLnode** paragraphs;
    int num_paragraphs;
} LoadedFile;

#endif // MENU_H
