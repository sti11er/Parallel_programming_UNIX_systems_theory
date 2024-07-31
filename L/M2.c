#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void 
handler(int signum) {
    static int count = 0;
    count++;
    printf("%d\n", count);
    if (count == 5) {
        exit(0);
    }
}

int 
main(void) {
    pid_t pid;
    if ((pid = fork()) == 0) {
        signal(SIGINT, handler);
        while (1);
    } else {
        for (int i = 0; i < 5; i++) {
            usleep(50);
            kill(pid, SIGINT);
        }
        wait(NULL);
    }
    return 0;
}