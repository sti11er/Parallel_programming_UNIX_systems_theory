#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

// p1 | p2 | ... | pn
int main(int argc, char *argv[]) {
    argc--; argv++;
    int fds[argc][2];
            
    pipe(fds[0]);
    if (fork() == 0) {  
        dup2(fds[0][1], STDOUT_FILENO);
        close(fds[0][0]);  
        close(fds[0][1]);
        execvp(argv[0], &argv[0]);
        exit(1);
    }

    for (int i=1; i<(argc-1); i++) {
        pipe(fds[i]);
        int pid;
        while((pid = fork()) == -1){
            wait(NULL);
        }
        if (!pid) {
            dup2(fds[i-1][0], STDIN_FILENO);    
            dup2(fds[i][1], STDOUT_FILENO);
            
            for (int j=0; j<(i+1); j++) {
                close(fds[j][1]);    
                close(fds[j][0]); 
            }

            execvp(argv[i], &argv[i]);
            exit(1);
        }
    }

    if (fork() == 0) {  
        dup2(fds[argc-2][0], STDIN_FILENO);
        close(fds[argc-2][0]);  
        close(fds[argc-2][1]);
        execvp(argv[argc-1], &argv[argc-1]);
        exit(1);
    }
    
    for (int i=0; i<argc; i++) {
        close(fds[i][1]);    
        close(fds[i][0]); 
    }
    while (wait(NULL) != -1);
    return 0;
}

