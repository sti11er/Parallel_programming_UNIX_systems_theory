#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void print_number(int num) {
    printf("%d\n", num);
}

int main(void) {
    pid_t pid1, pid2;
    int status;

    FILE *tmp;
    tmp=tmpfile();

    pid1 = fork();
    if (pid1 == 0) {
        print_number(2);
        fprintf(tmp, "%d", 1);
        exit(0);
    }

    waitpid(pid1, &status, 0);
    pid2 = fork();
    if (pid2 == 0) {
        int num;
        rewind(tmp);
        fscanf(tmp, "%d", &num);
        if (num == 1) {
            print_number(1);
            exit(0);
        }
    }

    waitpid(pid2, &status, 0);
    return 0;
}