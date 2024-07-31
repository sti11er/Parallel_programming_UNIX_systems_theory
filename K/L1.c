#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

// p1 args1... | p2 args2....
int main(int argc, char *argv[]) {
    argc--; argv++;
    int pipefd[2];
    pid_t p1, p2; 

    int sep = 1;
    while (sep < argc && strcmp(argv[sep++], "--") != 0);

    int size1 = sep, size2 = argc-sep+1;
    char *argv1[size1];
    char *argv2[size2];

    for (int i=0; i<size1-1; i++) {
        argv1[i] = argv[i];
    }
    argv1[size1-1] = NULL;
    for (int i=0; i<size2-1; i++) {
        argv2[i] = argv[i+sep];
    }
    argv2[size2-1] = NULL;

    pipe(pipefd);
    if ((p1 = fork()) == 0) {
        close(pipefd[0]);      
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        execvp(*argv1, argv1);
        exit(1);
    }

    if ((p2 = fork())== 0) {
        close(pipefd[1]);    
        dup2(pipefd[0], STDIN_FILENO);  
        close(pipefd[0]);     
        execvp(*argv2, argv2); 
        exit(1);
    }

    close(pipefd[0]); 
    close(pipefd[1]);
    waitpid(p1, NULL, 0);
    waitpid(p2, NULL, 0);
    return 0;
}
