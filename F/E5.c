#include <stdio.h>
#include <string.h>
#include <ctype.h>

enum { MAX_STRING_LENGTH = 80};

void form_array_of_strings(char **strings, char *string) {
    int i = 0; char *tmp = NULL;
    while (*string != '\0') {
        if (isalpha(*string) && tmp == NULL){
            tmp = string;
        }
        else if (!isalpha(*string) && (tmp != NULL)){  
            *string = '\0'; 
            strings[i++] = tmp;
            tmp = NULL;
        }
        string++; 
    }
    if (tmp != NULL) {
        strings[i++] = tmp;
        tmp = NULL;
    }
    strings[i] = NULL;
}

void print_max_len_str(char **strings) {
    char *substr = "begin"; 
    for (int i = 0; strings[i] != NULL; i++) {
        if (strstr(strings[i], substr) != NULL) {
            printf("%s\n", strings[i]);
        }
    }
}

int main(void) {
    char string[MAX_STRING_LENGTH + 1];
    char *strings[MAX_STRING_LENGTH / 2 + 1];
    char *ptr;
    fgets(string, sizeof(string) , stdin);
    ptr = strrchr(string, '\n');
    if (ptr != NULL) {
        *ptr = 0;
    }
    form_array_of_strings(strings, string);
    print_max_len_str(strings);
    return 0;
}