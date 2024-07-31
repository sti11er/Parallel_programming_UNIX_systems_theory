#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char * argv[])
{
    int status;
    int f = open(argv[3], O_WRONLY | O_CREAT | O_APPEND, 0666);
    
    if (fork()==0){
        dup2(f, 1);
        close(f);

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
    
    
    wait(&status);
    if ((WIFEXITED(status)) && (WEXITSTATUS(status) == 0)) {
        if (fork() == 0){
            execlp(argv[4], argv[4], NULL);
            return 1;
        }
        wait(&status);
    }

    return 0;
}