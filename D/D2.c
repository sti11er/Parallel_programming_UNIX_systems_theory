#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void new_strcat(char *s1, char *s2) {
    s1 = strchr(s1, '\n');
    while (*s2) {
        *s1 = *s2;
        s1++;
        s2++;
    }
    *s1 = '\0';
}

int main(void) {
    char str1[162], str2[82];
    fgets(str1, sizeof(str1), stdin);
    fgets(str2, sizeof(str2), stdin);
    new_strcat(str1, str2);
    printf("%s", str1);
}