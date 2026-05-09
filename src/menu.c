#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <termios.h>
#include "menu.h"

static LoadedFile loaded_files[20];
static int num_loaded_files = 0;
static AVLnode* last_result_tree = NULL;
static AVLnode** last_result_para_array = NULL;

//      memory management
static void free_tree(AVLnode* root) {
    if (!root) return;
    free_tree(root->LC);
    free_tree(root->RC);
    if (root->word) free(root->word);
    free(root);
}
static void cleanup_results() {
    if (last_result_tree) { free_tree(last_result_tree); last_result_tree = NULL; }
    if (last_result_para_array) { free(last_result_para_array); last_result_para_array = NULL; }
}
static void cleanup_files() {
    for (int i = 0; i < num_loaded_files; i++) {    
        for (int j = 0; j < loaded_files[i].num_paragraphs; j++) {
            free_tree(loaded_files[i].paragraphs[j]);
        }
        free(loaded_files[i].paragraphs);
    }
    num_loaded_files = 0;
}

//      flushes the buffer
static void flush_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

//      UI helpers 

static void print_avl_as_set_recursive(AVLnode* root, bool* first) {
    //  we reach null
    if (!root) return;

    //  traverse the avl
    print_avl_as_set_recursive(root->LC, first);

    //  check if it's the first sentence, and print ',' if it's not the first
    if (!*first) printf(", ");

    //  print the sentence
    printf("\"%s\"", root->word);
    *first = false;             //  set to false so after each sentence add a comma
    print_avl_as_set_recursive(root->RC, first);    //  traverse teh the right branch of the avl
}
static void print_avl_as_set(AVLnode* root) {

    //  just adds curly braces for each paragraph
    printf("{");
    bool first = true;
    print_avl_as_set_recursive(root, &first);
    printf("}");
}

//      print the set of paragraphs   
static void print_paras_as_set(AVLnode** paras) {
    printf("{ ");
    for (int i = 0; paras[i] != NULL; i++) {
        if (i > 0) printf(", \n    ");
        print_avl_as_set(paras[i]);
    }
    printf(" }");
}


void clear_screen() { printf("\033[H\033[J"); }

static int terminal_width() {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 0) return ws.ws_col;
    return 80;
}

static int ui_width() {
    int cols = terminal_width();
    if (cols < 56) return cols - 4;
    if (cols < 90) return cols - 10;
    return 78;
}

static void print_spaces(int count) {
    for (int i = 0; i < count; i++) printf(" ");
}

static int ui_indent(int width) {
    int indent = (terminal_width() - width - 2) / 2;
    return indent > 0 ? indent : 0;
}

void centered_text(char* text, int width) {
    int len = (int)strlen(text);
    int padding = (width - len) / 2;
    if (padding < 0) padding = 0;
    print_spaces(padding);
    printf("%s", text);
    print_spaces(width - len - padding > 0 ? width - len - padding : 0);
}

static void print_box_rule(int indent, int width, char* left, char* fill, char* right) {
    print_spaces(indent);
    printf(BOLD_CYAN "%s", left);
    for (int i = 0; i < width; i++) printf("%s", fill);
    printf("%s\n" RESET, right);
}

static void print_box_text(int indent, int width, char* text, char* color) {
    print_spaces(indent);
    printf(BOLD_CYAN "║" RESET);
    if (color) printf("%s", color);
    centered_text(text, width);
    if (color) printf(RESET);
    printf(BOLD_CYAN "║\n" RESET);
}

static void print_box_line(int indent, int width, char* left, char* right, char* color) {
    int left_len = (int)strlen(left);
    int right_len = right ? (int)strlen(right) : 0;
    int left_limit = width - right_len - 1;
    if (left_limit < 0) {
        right_len = 0;
        right = "";
        left_limit = width;
    }
    if (left_len > left_limit) left_len = left_limit;
    int gap = width - left_len - right_len;
    if (gap < 0) gap = 0;

    print_spaces(indent);
    printf(BOLD_CYAN "║" RESET);
    if (color) printf("%s", color);
    printf("%.*s", left_len, left);
    print_spaces(gap);
    if (right) printf("%s", right);
    if (color) printf(RESET);
    printf(BOLD_CYAN "║\n" RESET);
}

static void print_mini_header(char* title) {
    int width = ui_width();
    int indent = ui_indent(width);

    printf("\n");
    print_box_rule(indent, width, "╔", "═", "╗");
    print_box_text(indent, width, "ESI  |  ADDS Lab Work  |  Bensefia  |  Cherimekh", WHITE);
    if (title) {
        print_box_rule(indent, width, "╠", "═", "╣");
        print_box_text(indent, width, title, BOLD_WHITE);
    }
    print_box_rule(indent, width, "╚", "═", "╝");
}

void print_separator(char* title) {
    print_mini_header(title);
    printf("\n");
}

static void print_prompt(char* prompt) {
    print_spaces(ui_indent(ui_width()) + 2);
    printf(BOLD_WHITE "%s " RESET, prompt);
}

static void print_status(char* msg, char* color) {
    print_spaces(ui_indent(ui_width()) + 2);
    if (color) printf("%s", color);
    printf("%s", msg);
    if (color) printf(RESET);
}
void loading_animation(char* msg) {
    char* frames[] = {"|", "/", "-", "\\"};
    int indent = ui_indent(ui_width());
    print_spaces(indent + 2);
    printf(BOLD_WHITE "%s " RESET, msg);
    for (int i = 0; i < 10; i++) {
        printf(CYAN "%s" RESET "\b", frames[i % 4]); fflush(stdout);
        usleep(80000);
    }
    printf(" Done!\n");
}

static void wait_for_key() {
    printf("\n");
    int width = ui_width();
    int indent = ui_indent(width);
    print_box_rule(indent, width, "╔", "═", "╗");
    print_box_text(indent, width, "Press any key to continue...", YELLOW);
    print_box_rule(indent, width, "╚", "═", "╝");
    fflush(stdout);

    if (isatty(STDIN_FILENO)) {
        struct termios oldt, newt;
        if (tcgetattr(STDIN_FILENO, &oldt) == 0) {
            newt = oldt;
            newt.c_lflag &= ~(ICANON | ECHO);
            tcsetattr(STDIN_FILENO, TCSANOW, &newt);
            getchar();
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            return;
        }
    }
    getchar();
}

void splash_screen() {
    clear_screen();
    int width = ui_width();
    int indent = ui_indent(width);

    printf("\n\n");
    print_box_rule(indent, width, "╔", "═", "╗");
    print_box_text(indent, width, "ECOLE NATIONALE SUPERIEURE D'INFORMATIQUE", GRAY);
    print_box_rule(indent, width, "╠", "═", "╣");
    print_box_text(indent, width, " _______   ________  ___         ", BOLD_CYAN);
    print_box_text(indent, width, "|\\  ___ \\ |\\   ____\\|\\  \\        ", BOLD_CYAN);
    print_box_text(indent, width, "\\ \\   __/|\\ \\  \\___|\\ \\  \\       ", BOLD_CYAN);
    print_box_text(indent, width, " \\ \\  \\_|/_\\ \\_____  \\ \\  \\      ", BOLD_CYAN);
    print_box_text(indent, width, "  \\ \\  \\_|\\ \\|____|\\  \\ \\  \\     ", BOLD_CYAN);
    print_box_text(indent, width, "   \\ \\_______\\____\\_\\  \\ \\__\\    ", BOLD_CYAN);
    print_box_text(indent, width, "    \\|_______|\\_________\\|__|    ", BOLD_CYAN);
    print_box_text(indent, width, "             \\|_________|        ", BOLD_CYAN);
    print_box_text(indent, width, "                                 ", BOLD_CYAN);
    print_box_rule(indent, width, "╠", "═", "╣");
    print_box_text(indent, width, "ADDS LAB WORK", BOLD_WHITE);
    print_box_text(indent, width, "Set Operations on Text Files", YELLOW);
    print_box_rule(indent, width, "╟", "─", "╢");
    print_box_text(indent, width, "Presented by", GRAY);
    print_box_text(indent, width, "Cherimekh Samy Firas", WHITE);
    print_box_text(indent, width, "Bensefia Abdessamed", WHITE);
    print_box_rule(indent, width, "╟", "─", "╢");
    print_box_line(indent, width, "  Module", "ADDS  ", WHITE);
    print_box_line(indent, width, "  Academic Work", "Lab Work  ", WHITE);
    print_box_line(indent, width, "  Group", "02  ", YELLOW);
    print_box_rule(indent, width, "╚", "═", "╝");
    wait_for_key();
}

//      menu logic

void file_loading_flow() {

    /*      STEP 1 :  loading files    */
    cleanup_files();
    clear_screen();
    print_separator("FILE LOADING");
    
    int count;      //      indicates the number of files loaded
    print_prompt("How many files to load (1-20)?");

    //      this ensures that the number of files loaded is valid 
    while (scanf("%d", &count) != 1 || count < 1 || count > 20) {
        print_status("Invalid entry (1-20): ", RED); flush_stdin();
    }
    flush_stdin();

    
    for (int i = 0; i < count; i++) {
        char path[256];
        while (1) {
            print_spaces(ui_indent(ui_width()) + 2);
            printf(BOLD_WHITE "File %d path: " RESET, i + 1);

            //      reads the path from the user
    
            if (!fgets(path, sizeof(path), stdin)) continue;
            path[strcspn(path, "\n")] = 0;      //  delets the null terminates to read the path correctly
            
            FILE* f = fopen(path, "r");
            if (f) {
                fclose(f);
                loading_animation("Parsing content...");
                int num = 0;

                //  insert paragraphs into an array of avl trees
                AVLnode** paras = GetParagraphs(path, &num);    //  store the the paragraph
                if (paras) {
                    char* fn = strrchr(path, '/');      //  extracts the name of the file path
                    strncpy(loaded_files[num_loaded_files].name, fn ? fn + 1 : path, 255);  //  add the name of the file
                    loaded_files[num_loaded_files].paragraphs = paras;          //      add the paragraph
                    loaded_files[num_loaded_files].num_paragraphs = num;        //      add the number of paragarphs    
                    num_loaded_files++;     //  update the number of files loaded
                    print_spaces(ui_indent(ui_width()) + 2);
                    printf(GREEN "[OK] Loaded %d paragraphs.\n" RESET, num);
                    break;      //  getting out of the condition
                }
            }

            //      if the path isn't correct  
            print_status("Could not open file. Try again.\n", RED);
        }
    }
    //      file uploading done
    printf("\n");
    print_prompt("Press Enter...");
    getchar();
}

static void show_previews(int idx) {
    
    int width = ui_width();
    int indent = ui_indent(width);
    char title[320];
    snprintf(title, sizeof(title), "Paragraphs in %s", loaded_files[idx].name);
    printf("\n");
    print_box_rule(indent, width, "╔", "═", "╗");
    print_box_text(indent, width, title, BOLD_WHITE);
    print_box_rule(indent, width, "╟", "─", "╢");

    for (int i = 0; i < loaded_files[idx].num_paragraphs; i++) {
        //      show the paragraphs of the file  
        char* s = calloc(1, 1); 
        getFullParagraph(loaded_files[idx].paragraphs[i], &s);  //  store the paragraph in s
        char line[512];
        snprintf(line, sizeof(line), "  [%d] %.60s...", i + 1, s);
        print_box_line(indent, width, line, "", WHITE);                  //  print s (not fully)  
        free(s); 
    }
    print_box_rule(indent, width, "╚", "═", "╝");

}

//      handles user input with conditions
static int select_val(char* prompt, int min, int max) {
    int val;
    print_prompt(prompt);
    while (scanf("%d", &val) != 1 || val < min || val > max) {
        print_spaces(ui_indent(ui_width()) + 2);
        printf(RED "Invalid choice (%d-%d): " RESET, min, max);
        flush_stdin(); 
    }
    return val;
}

void operation_flow() {
    bool chaining = false;      //  indicates if we're chaining the result or give result directly

    while (1) {
        clear_screen();
        print_separator(chaining ? "CHAINING OPERATION" : "SET OPERATION"); // if true we're chaining else we're in a fresh opertation
        char level = 'S', op;

        /*                  part 01                   */

        /*                  01 
                get set op level from the user 
            appears only if there are more than two paragraphs */
        if (!chaining && num_loaded_files > 1) {
            print_status("[S] Sentence level    [P] Paragraph level\n", YELLOW);
            print_prompt("Choice:");

            //  ensure it's sentence level or para level
            while (scanf(" %c", &level) && (level = toupper(level)) && level != 'S' && level != 'P') 
                print_status("Enter S or P: ", RED);
        }

        int f1 = 0, p1 = 0, f2 = 0, p2 = 0;

        if (level == 'S') {
            
            if (!chaining) {

                /*              part 2
                                  01
                        getting the first operand  */
                if (num_loaded_files > 1) f1 = select_val("Select file (first operand): ", 1, num_loaded_files) - 1;       //  get the index of the fiile
                show_previews(f1);      // index of file 
                p1 = select_val("Select paragraph (first operand):", 1, loaded_files[f1].num_paragraphs) - 1;   //  get the paragraph
            }
                /*              02
                        choose the operation    */
            printf("\n");
            print_status("[U] Union    [I] Intersection    [D] Difference\n", YELLOW);
            print_prompt("Op:");
            while (scanf(" %c", &op) && (op = toupper(op)) && op != 'U' && op != 'I' && op != 'D')
                print_status("Enter U, I, or D: ", RED);

                /*              03
                        getting the second operand     */
            f2 = select_val("Select file (second operand):", 1, num_loaded_files) - 1;
            show_previews(f2);
            p2 = select_val("Select paragraph (second operand):", 1, loaded_files[f2].num_paragraphs) - 1;

            loading_animation("Processing...");
            AVLnode* res = NULL;
            AVLnode* left = chaining ? last_result_tree : loaded_files[f1].paragraphs[p1];
            AVLnode* right = loaded_files[f2].paragraphs[p2];

                /*              04
                        performing set operation on operands   */
            if (op == 'U') UNION_SENT(left, right, &res);
            else if (op == 'I') INTERSECTION_SENT(left, right, &res);
            else DIFFERENCE_SENT(left, right, &res);

            char label_l[300], label_r[300];

            /*                  part 3
                                  01
                presenting in a human readable way the operands of the operation     */
            if (chaining) strcpy(label_l, "Previous_Result");
            else sprintf(label_l, "%s[P%d]", loaded_files[f1].name, p1 + 1);
            sprintf(label_r, "%s[P%d]", loaded_files[f2].name, p2 + 1);

            char* op_str = (op == 'U') ? "∪" : (op == 'I' ? "∩" : "\\");    //  mathematical representation of the set operations 

            /*                    02
                display the result of the operation     */
            printf("\n");
            print_status("RESULT\n", BOLD_CYAN);
            print_spaces(ui_indent(ui_width()) + 2);
            printf(BOLD_WHITE "%s %s %s = " RESET, label_l, op_str, label_r);
            print_avl_as_set(res);  //  print result AVL tree in a set representation
            printf("\n");

            if (chaining) free_tree(last_result_tree);
            last_result_tree = res;
            printf("\n");
            print_status("Done. Result stored.\n", GREEN);

        } else {      //    paragraph level       
            //  get the first operand
            if (!chaining) f1 = select_val("Select the file (first operand):", 1, num_loaded_files) - 1;
            
            //  choose the operation
            printf("\n");
            print_status("[U] Union    [I] Intersection    [D] Difference\n", YELLOW);
            print_prompt("Op:");
            
            while (scanf(" %c", &op) && (op = toupper(op)) && op != 'U' && op != 'I' && op != 'D')
                print_status("Enter U, I, or D: ", RED);
            
            //  get the second operand    
            f2 = select_val("Select the file (second operand): ", 1, num_loaded_files) - 1;

            loading_animation("Processing...");
            int max = 100; // Simplified
            AVLnode** res = calloc(max, sizeof(AVLnode*));
            AVLnode** left = chaining ? last_result_para_array : loaded_files[f1].paragraphs;
            
            //  perform the set operation
            if (op == 'U') UNION_PARA(left, loaded_files[f2].paragraphs, res);
            else if (op == 'I') INTERSECTION_PARA(left, loaded_files[f2].paragraphs, res);
            else DIFFERENCE_PARA(left, loaded_files[f2].paragraphs, res);

            //  show result
            char label_l[300], label_r[300];
            if (chaining) strcpy(label_l, "Previous_Result");
            else sprintf(label_l, "File:%s", loaded_files[f1].name);
            sprintf(label_r, "File:%s", loaded_files[f2].name);
            char* op_str = (op == 'U') ? "∪" : (op == 'I' ? "∩" : "\\");

            printf("\n");
            print_status("RESULT\n", BOLD_CYAN);
            print_spaces(ui_indent(ui_width()) + 2);
            printf(BOLD_WHITE "%s %s %s = " RESET, label_l, op_str, label_r);
            print_paras_as_set(res);
            printf("\n");

            if (chaining) free(last_result_para_array);
            last_result_para_array = res;
            printf("\n");
            print_status("Done. Result stored.\n", GREEN);
        }

        printf("\n");
        print_status("[C] Chain    [N] New    [R] Reload    [Q] Quit\n", YELLOW);
        print_prompt("Choice:");
        
        char next; 
        scanf(" %c", &next); 
        next = toupper(next);

        /*          
                    providing options to the user
        */

        if (next == 'C') chaining = true;
        else if (next == 'N') { chaining = false; cleanup_results(); }
        else if (next == 'R') { file_loading_flow(); chaining = false; cleanup_results(); }
        else { cleanup_results(); break; }
    }
}
void main_menu() {
    while (1) {
        clear_screen();
        print_separator("MAIN MENU");
        int width = ui_width();
        int indent = ui_indent(width);
        print_box_rule(indent, width, "╔", "═", "╗");
        print_box_text(indent, width, "ADDS - SET OPERATIONS", BOLD_WHITE);
        print_box_rule(indent, width, "╠", "═", "╣");
        print_box_line(indent, width, "  [1] Load Files", "", WHITE);
        char* color = num_loaded_files ? WHITE : GRAY;
        print_box_line(indent, width, "  [2] Perform Operation", num_loaded_files ? "" : "Load files first  ", color);
        print_box_line(indent, width, "  [3] View Loaded Files", num_loaded_files ? "" : "Load files first  ", color);
        print_box_line(indent, width, "  [0] Exit", "", WHITE);
        print_box_rule(indent, width, "╚", "═", "╝");
        
        int choice;
        printf("\n");
        print_prompt("Choice:");
        if (scanf("%d", &choice) != 1) { flush_stdin(); continue; }
        if (choice == 1) file_loading_flow();
        else if (choice == 2 && num_loaded_files) operation_flow();
        else if (choice == 3 && num_loaded_files) {
            clear_screen(); print_separator("LOADED FILES");
            int files_width = ui_width();
            int files_indent = ui_indent(files_width);
            print_box_rule(files_indent, files_width, "╔", "═", "╗");
            for (int i=0; i<num_loaded_files; i++) {
                char file_line[360];
                snprintf(file_line, sizeof(file_line), "  [%d] %s", i+1, loaded_files[i].name);
                char count_line[64];
                snprintf(count_line, sizeof(count_line), "%d paragraphs  ", loaded_files[i].num_paragraphs);
                print_box_line(files_indent, files_width, file_line, count_line, WHITE);
            }
            print_box_rule(files_indent, files_width, "╚", "═", "╝");
            printf("\n");
            print_prompt("Press Enter...");
            flush_stdin(); getchar();
        }
        else if (choice == 0) { cleanup_files(); cleanup_results(); break; }
    }
}
void run_app() { splash_screen(); main_menu(); }
