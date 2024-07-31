#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

void print_add(char* s) {
    printf("ADD\n");
}

void print_sub(char* s) {
    printf("SUB\n");
}

void check_number(char* s) {
    char* endptr; long res;
    res = strtol(s, &endptr, 10);
    if (*endptr == '\0' && res >= INT_MIN && res <= INT_MAX && strlen(s) != 0) {
        printf("NUMBER\n");
    } else {
        printf("UNKNOWN\n");
    }
}

typedef struct {
    char* str; 
    void (*func)(char*); 
} string_switch;

void execute_string_switch(string_switch* ss, char* s) {
    if (s == NULL) { printf("UNKNOWN\n"); return; }
    while (ss->str != NULL) {
        if (strcmp(ss->str, s) == 0) {
            ss->func(s);
            return;
        }
        ss++;
    }
    ss->func(s);
}

int main(int argc, char** argv) {
    string_switch ss[] = {{"add", print_add}, {"sub", print_sub}, {NULL, check_number}};
    

    for (int i = 1; i < argc; i++) {
        execute_string_switch(ss, argv[i]);
    }
    return 0;
}