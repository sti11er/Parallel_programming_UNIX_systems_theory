#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void 
processA() {
    int a, b;
    while (scanf("%d %d", &a, &b) == 2) {
        printf("%d %d\n", a + b, a - b);
        fflush(stdout);
    }
}

void 
processB(int read_fd, int write_fd) {
    int a, b;
    while (scanf("%d %d", &a, &b) == 2) {
        dprintf(write_fd, "%d %d\n", a, b);
        fflush(stdout);
        int sum, diff;
        scanf("%d %d", &sum, &diff);
        printf("%d %d %d %d\n", a, b, sum, diff);
        fflush(stdout);
    }
}

int 
main() {
    int fd[2];
    pipe(fd);
    int read_fd = fd[0];
    int write_fd = fd[1];

    pid_t pid = fork();
    if (pid == 0) {
        close(read_fd);
        processB(STDIN_FILENO, write_fd);
        close(write_fd);
        exit(0);
    } else if (pid > 0) {
        close(write_fd);
        processA(read_fd);
        close(read_fd);
        wait(NULL);
        return 0;
    } else {
        printf("Failed to fork\n");
        return 1;
    }
}