#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void value_reversal(char *str) {
    int len = strlen(str);
    char *new_str = (char*) calloc(len, sizeof(char));
    char *start, *end, *current;
    current = str; start = new_str; end = new_str+len-2;
    
    for (int i = 0; (i < len-1); i++) {
        if (isdigit(*current)) { 
            *start = *current; 
            start++;
        }
        else {
            *end = *current;
            end--;
        }
        current++;
    }
    printf("%s\n", new_str);
    free(new_str);
}

int main(void) {
    char str[82];
    fgets(str, 82, stdin);
    value_reversal(str);
    return 0;
}
