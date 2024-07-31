#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void add(char *str) {
    static int count = 0;
    if (++count % 2 == 1) {
        printf("%d\n", count);
    }
}

void sub(char *str) {
    static int count = 0;
    if (++count == 2) {
        exit(0);
    }
}

void default_case(char *str) {}

void execute_string_switch(char *str) {
    if (strcmp(str, "add") == 0) {
        add(str);
    } else if (strcmp(str, "sub") == 0) {
        sub(str);
    } else {
        default_case(str);
    }
}

int main(int argc, char **argv) {
    argc--; argv++;
    for (int i = 0; i < argc; i++) {
        execute_string_switch(argv[i]);
    }
    return 0;
}