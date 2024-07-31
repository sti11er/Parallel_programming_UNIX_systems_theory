#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare(const void *a, const void *b) {
    return strcmp(*(const char **)b, *(const char **)a);
}
int main(int argc, char *argv[]) {
    qsort(argv + 1, argc - 1, sizeof(char *), compare);
    for (int i = 1; i < argc; i++) {
        printf("%s\n", argv[i]);
    }
    
    qsort(argv + 1, argc - 1, sizeof(char *), compare);
    for (int i = argc - 1; i >= 1; i--) {
        if (strlen(argv[i]) % 2 == 0) {
            printf("%s\n", argv[i]);
        }
    }
    for (int i = 1; i < argc; i++) {
        if (strlen(argv[i]) % 2 != 0) {
            printf("%s\n", argv[i]);
        }
    }
    return 0;
}