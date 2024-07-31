#include <stdio.h>
#include <string.h>
#define BUF_SZ 82

int main(void) {
    char message[BUF_SZ];

    if (fgets(message, BUF_SZ, stdin) == NULL) {
        printf("EMPTY INPUT\n");
    } else {
        printf("%s", message);
    }
    
    return 0;
}