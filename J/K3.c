#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
    dup2(fd, STDOUT_FILENO);
    close(fd);
    if (fork() == 0) {
        execlp(argv[1], argv[1], NULL);
    }
    int status;
    wait(&status);
    return 0;
}