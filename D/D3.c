#include <stdio.h>
#include <string.h>

int compare(const char *needle, const char *haystack) {
    int len1 = strlen(needle); int len2 = strlen(haystack);
    if (len1 > len2) { return 0; }
    char *current = (char *)haystack+len2-len1;
    return !strcmp(current, needle) ?  1 : 0;
}

int main(void)
{
    char str1[82], str2[82];
    fgets(str1, sizeof(str1), stdin);
    fgets(str2, sizeof(str2), stdin);
    str1[strlen(str1)-1] = '\0';
    str2[strlen(str2)-1] = '\0';
    printf("%s\n", compare(str1, str2) ? "YES": "NO");
    return 0;
}
