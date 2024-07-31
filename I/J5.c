// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <sys/wait.h>

// int main(void) {
//     setbuf(stdin, 0);
//     int c; int status; 
//     int first = getchar(); 
//     while ((c = getchar()) != EOF) {
//         if (fork() == 0) {
//             if (first == c) {
//                 const char str[3] = {(char)c, (char)c, '\0'};
//                 printf("%s", str);
//             }
//             return 0;
//         } 
//     }
    
//     while (wait(&status) != -1){}
//     putchar('\n');
    
//     return 0;
// }

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    setbuf(stdin, 0);
    int c; int status;
    int first = getchar();
    while ((c = getchar()) != EOF) {
        int pid;
        while ((pid = fork()) == -1);
        if (pid == 0) {
            if (first == c) {
                const char str[3] = {c, c, '\0'};
                printf("%s", str);
            }
        return 0;
        }
    }

    while (wait(&status) != -1){}
    putchar('\n');
    return 0;
}