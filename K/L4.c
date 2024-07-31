#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    int fd[2];
    char buf; 

    pipe(fd);

    if (fork() == 0) { 
        close(fd[0]);
        int count = 1;
        while (read(STDIN_FILENO, &buf, 1) > 0) {
            if (count % 2 == 0) {
                write(fd[1], &buf, 1);
            }
            count++;
        }
        close(fd[1]);
        exit(0);
    }

    if (fork() == 0) { 
        close(fd[1]);
        if (fork() == 0) { 
            int count = 1;
            while (read(fd[0], &buf, 1) > 0) {
                if (count % 2 == 0) {
                    write(STDOUT_FILENO, &buf, 1);
                }
                count++;
            }
            close(fd[0]);
            exit(0);
        }

        wait(NULL);
        close(fd[0]);
        exit(0);
    }

    close(fd[0]);
    close(fd[1]);
    wait(NULL);
    wait(NULL);
    return 0;
}