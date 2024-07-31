#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *read_line(void) {
    int size = 10;
    char *buffer = malloc(size);
    if (buffer == NULL) {
        return NULL;
    }
    int len = 0; // current len string
    char *tmp = buffer; // point on free space
    while (1){
        if (fgets(tmp, size-len, stdin) == NULL) {
            free(buffer);
            return NULL;
        }
        len += strlen(tmp);
        if (len > 0 && buffer[len-1] == '\n') {
            break;
        }
        size *= 2;
        buffer = realloc(buffer, size);
        if (buffer == NULL) {
            return NULL;
        }
        tmp = buffer + len;
    }
    buffer[len] = '\0';
    return buffer;
}

int main(void)
{
    char *line = NULL;
    char *max_line = NULL;
    int max_len = 0;
    int len;
    
    
    while ((line = read_line()) != NULL) {
        len = strlen(line);
        if (len >= max_len) {
            free(max_line);
            max_line = line;
            max_len = len;
        } else {
            free(line);
        }
    }
    if (max_line != NULL) {
        printf("%s", max_line);
        free(max_line);
    }
    
    return 0;
}
