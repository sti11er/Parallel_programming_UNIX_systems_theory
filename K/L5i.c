#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>

#define SIZE_I sizeof (int)

void
process_A(int fd_mes[2], int fd_connect[2])
{
    int pid;
    if (!(pid = fork())){
            // process A
        int num[2]; // massive of numbers
        int num_out[2];
        close(fd_connect[0]); // unusble
        close(fd_mes[1]);
        while (read(fd_mes[0], num, sizeof num) == sizeof num){
            num_out[0] = num[0] + num[1]; // sum
            num_out[1] = num[0] - num[1]; // sub
            //write(fd_mes[1], num_out, sizeof num_out);
            write(fd_connect[1], num_out, sizeof num_out);
        }
        exit(0);
    }
    return;
}

void
son_code(int fd_mes[2], int fd_queue[2], int fd_connect[2], int num[2])
{
    int pid;
    while((pid = fork()) == -1){
        wait(NULL);
    }
    if (!pid) {
        char x;
        int num_cpy[2] = {num[0], num[1]};
        close(fd_connect[1]);
        close(fd_mes[0]);
            // will realise like deykstra semaphore
        read(fd_queue[0], &x, sizeof x); //down
        
            //must push a, b then pull
        write(fd_mes[1], num_cpy, sizeof num_cpy); // push numbers

        printf("%d %d ", num_cpy[0], num_cpy[1]);
        read(fd_connect[0], num_cpy, sizeof num_cpy);
        
        //read(fd_mes[0], num_cpy, sizeof num_cpy);
        printf("%d %d\n", num_cpy[0], num_cpy[1]);

        write(fd_queue[1], &x, sizeof x);
        //up
        //close all file descriptors in exit
        exit(0);
    }
    return;
}

int
main(void)
{
    int num[2]; // massive of a b numbers
    char x = 1;
    int fd_mes[2];
    int fd_queue[2];
    int fd_connect[2];
    
    pipe(fd_mes);
    pipe(fd_connect);
    setbuf(stdin, 0);

    process_A(fd_mes, fd_connect);
    pipe(fd_queue);
    write(fd_queue[1], &x, sizeof x); //semaphore = 1;
    while(scanf("%d%d",&num[0], &num[1]) == 2) {
        // creating  a son
        son_code(fd_mes, fd_queue, fd_connect, num);
    }
    close(fd_mes[1]); //exactly here
    close(fd_mes[0]); //        cause else process have chance to exit
    close(fd_queue[0]);
    close(fd_queue[1]);
    close(fd_connect[0]);
    close(fd_connect[1]);

    while(wait(NULL) != -1);
    return 0;
}