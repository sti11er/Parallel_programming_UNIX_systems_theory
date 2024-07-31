#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *my_bsearch(void *key, void *base, size_t nmemb,
        size_t size, int (*compare)(const void *, const void *))
{ 
    if (nmemb == 0) { return NULL; }
    size_t l = 0, r = nmemb-1;

    while (l <= r)
    {
        size_t m = l + (r - l) / 2;
        void *q = (char *)base + m*size;
        int cmp = compare(key, q);

        if (cmp < 0) {
            r = m - 1;
        } else if (cmp > 0){
            l = m + 1;
        } else {
            return q;
        }
    }

    return NULL;
}

int cmp(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

int cmp1(const void *a, const void *b) {
    return strcmp((char*)a, (*(char**)b));
}

int main(int argc, char *argv[])
{
    argv++; argc--;
    qsort(argv, argc, sizeof(char *), cmp);

    char s[256];
    if (fgets(s, sizeof(s), stdin) == NULL) {
        return 1;
    }
    s[strcspn(s, "\n")] = '\0';

    for (int i = 0; i < argc; i++) {
        printf("%s ", argv[i]);
    } 
    printf("\n");
    
    void *result = my_bsearch(s, argv, argc, sizeof(char *), cmp1);
    
    if (result != NULL) {
        for (int i = 0; i < argc; i++) {
            if (cmp1(argv[i],result)==0) {
                printf("%d\n", i+1);
                break;
            }
        }
    } else {
        printf("0\n");
    }
    return 0;
}
