#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

int num; char *str1; char *str2;
time_t last_print_time;

void 
handler1(int signum) {
    time_t current_time = time(NULL);
    int seconds = current_time - last_print_time;
    printf("%d\n", 5 - seconds);
}

void 
handler2(int signum) {
    time_t current_time = time(NULL);
    last_print_time = current_time;
    if (num == 1) {
        printf("%s\n", str1);
        fflush(stdout);
        if (strcmp(str1, "0") != 0) {
            alarm(5);
        }
    } else if (num == 2) {
        printf("%s\n", str2);
        fflush(stdout);
        if (strcmp(str2, "0") != 0) {
            alarm(5);
        }
    }
}

// char *
// dec(char *s) {
//     char *tmp = s;
//     int len = strlen(s);
//     int i = len - 1;
//     while ((i >= 0) && (s[i] == '0')) {
//         s[i] = '9';
//         i--;
//     }
//     if (i == -1) {
//         s[len - 1] = '0';
//         return s + len - 1;
//     }
//     s[i]--;
//     s = tmp;
// }

int 
main(int argc, char *argv[]) {
    str1 = argv[1];
    str2 = (char *)malloc(strlen(str1) + 1);
    signal(SIGINT, handler1);
    signal(SIGALRM, handler2);
    
    last_print_time = time(NULL);
    alarm(5);
    // char as[strlen(str1)+1];
    // int i;
    // for (i=0; i<strlen(str1); i++) {
    //     as[i] = '0';
    // }
    // as[i] = '\0';
    while (strcmp(str1, "0") != 0) {
        num = 1;
        dec(str1);
        num = 2;
        strcpy(str2, str1);
    }
    free(str2);
    return 0;
}

