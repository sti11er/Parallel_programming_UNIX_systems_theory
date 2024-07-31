#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        printf("%s\n", argv[i]);
    }
    char *tmp1, *tmp2;
    for (int i = 1; i < argc; i++) {
        tmp1 = strstr(argv[i], "end");
        if (tmp1 != NULL) {
            tmp2 = strstr(tmp1+3, "end");
            if (tmp2 != NULL) {
                printf("%s\n", tmp2+3);
            }
        }
    }
    return 0;
}