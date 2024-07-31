#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>

// grep '^[a-zA-Z_][a-zA-Z0-9_]*\(' d.c | cut -d \( -f 1 | sort 

int
main(void)
{
    int fd[2];
    int pipes[2];

    pipe(fd);
    pipe(pipes);
    if (fork() == 0) {        
        dup2(fd[1], 1);
        close(fd[1]);
        close(fd[0]);
        close(pipes[0]);
        close(pipes[1]);
        execlp("grep", "grep", "^[[:alnum:]_]\\+(.*)$", NULL);
        exit(1);
    }
    if (fork() == 0) {
        dup2(fd[0],0);
        dup2(pipes[1],1);
        close(fd[1]);
        close(fd[0]);
        close(pipes[0]);
        close(pipes[1]);
        execlp("cut", "cut", "-d","(","-f1", NULL);
        exit(1);
    }
    
    if (fork() == 0) {
        dup2(pipes[0],0);
        close(fd[1]);
        close(fd[0]);
        close(pipes[0]);
        close(pipes[1]);
        execlp("sort", "sort", NULL);
        exit(1);
    }

    close(fd[1]);
    close(fd[0]);
    close(pipes[0]);
    close(pipes[1]);

    while(wait(NULL) != -1);
    return 0;
}