#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int status;
    char *p2 = argv[argc-1];
    argv[argc-1] = NULL;

    if (fork() == 0) {
        execvp(argv[1], &argv[1]);
    } else {
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            if (fork() == 0) {
                execlp(p2, p2, NULL);
            } else {
                wait(&status);
            }
        }
    }
    return 0;
}