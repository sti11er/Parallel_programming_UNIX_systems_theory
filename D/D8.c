#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct char_num {
    int size;
    char *num;
} tchar_num;

int compare_num(tchar_num struct_str1, tchar_num struct_str2) { 
    char *str1 = struct_str1.num,  *str2= struct_str2.num;
    int size1 = struct_str1.size,  size2= struct_str2.size;
    if (size1 > size2) { return 0; }
    if (size1 < size2) { return 1; }
    for (int i = 0; i < size1; i++) {
        if (*str1 > *str2) {
            return 0;
        }
    }
    return 1;
}

void left_search_num(char *str, tchar_num *ptr_num){
    ptr_num->num = NULL;
    ptr_num->size = 0;
    for (char *p = str; *p != '\0'; p++) {
        if (*p >= '0' && *p <= '9') { 
            if (ptr_num->num == NULL) {
                ptr_num->num = str;
            }
            (ptr_num->size)++;
        }
        else { break; }
    }
}

void find_max_num(char *str, tchar_num *ptr_max_num) {
    ptr_max_num->num = NULL;
    ptr_max_num->size = 0;
    tchar_num current;

    for (char *p = str; *p != '\0'; p++) {
        left_search_num(p, &current);
        p+=current.size;
        int response = compare_num(*ptr_max_num, current);
        if (response) {
            *ptr_max_num = current;
        }
    }
} 

int main(void)
{
    tchar_num ans;
    char* str = (char*)calloc(82, sizeof(char));
    fgets(str, 82, stdin);
    find_max_num(str, &ans);

    int c = -1;
    for (int i = 0; i < ans.size; i++) {
        if (ans.num[i] != '0') {
            c = i;
            break;
        }
    }

    if (ans.size == 0) { printf("\n"); }
    else if (c == -1) { printf("0\n"); }
    else {
        for (int i = c; i < ans.size; i++) {
            printf("%c", ans.num[i]);
        }
        printf("\n");
    }
    
    free(str);
    return 0;
}