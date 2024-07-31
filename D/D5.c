#include <stdio.h>
#include <stdbool.h>
#include <string.h>

bool check_str(const char* str1, const char* str2) {
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    int i = 0, j = 0;
    while (i < len1 && j < len2) {
        if (str1[i] == ' ') {
            i++;
            continue;
        }
        if (str2[j] == ' ') {
            j++;
            continue;
        }
        if (str1[i] != str2[j]) {
            return false;
        }
        i++;
        j++;
    }

    while (i < len1 && str1[i] == ' ') { i++; }
    while (j < len2 && str2[j] == ' ') { j++; }

    return i == len1 && j == len2;
}

int main(void) {
    char str1[82], str2[82];
    fgets(str1, sizeof(str1), stdin);
    fgets(str2, sizeof(str2), stdin);
    
    bool result = check_str(str1, str2);
    
    if (result) {
        printf("YES\n");
    } else {
        printf("NO\n");
    }
    
    return 0;
}