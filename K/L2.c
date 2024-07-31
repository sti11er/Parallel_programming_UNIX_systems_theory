#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

// (p1 || p2) | p3 args....
// ./L2 p1 p2 -- p3 args...
int main(int argc, char *argv[]) {
    int fd[2]; int status;

    int size = argc - 2;
    char *argv_p3[size];

    for (int i=0; i<size-1; i++) {
        argv_p3[i] = argv[3+i];
    }
    argv_p3[size-1] = NULL;

    pipe(fd);
    if (fork()==0){
        close(fd[0]);      
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        
        if (fork()==0) {
            execlp(argv[1], argv[1], NULL);
            exit(1);
        }
        wait(&status);
        if ((WIFEXITED(status)) && (WEXITSTATUS(status) == 0)){
            return WEXITSTATUS(status);
        }
        if (fork() == 0){
            execlp(argv[2], argv[2], NULL);
            exit(1);
        }
        wait(&status);
        if (WIFEXITED(status) && (WEXITSTATUS(status) == 0)){
            return WEXITSTATUS(status);
        } 
        return 1;
    }

    if (fork()== 0) {
        close(fd[1]);    
        dup2(fd[0], STDIN_FILENO);  
        close(fd[0]);     
        execvp(argv[3], argv_p3); 
        exit(1);
    }

    close(fd[0]); 
    close(fd[1]);
    wait(&status);
    wait(&status);
    return 0;
}
