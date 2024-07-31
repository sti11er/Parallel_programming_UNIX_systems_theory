#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void 
processA(int a_in, int a_out) {
    int buf[2]; int new_buf[2];
    while (read(a_in, &buf, sizeof(int)*2) > 0) {
        new_buf[0] = buf[0]+buf[1]; 
        new_buf[1] = buf[0]-buf[1]; 
        write(a_out, &new_buf, sizeof(int)*2);
    }
}

void 
son(int a_in, int a_out, int son_in, int son_out, int a, int b) {
    char c;
    read(son_in, &c, sizeof(char));
    int buf[2] = {a, b}; 
    write(a_in, &buf, sizeof(int)*2);
    read(a_out, &buf, sizeof(int)*2);
    printf("%d %d %d %d\n", a, b, buf[0], buf[1]);
    write(son_out, &c, sizeof(char));
}

int 
main(void) {
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);
    int pipe_for_son[2];
    int a_pipe_in[2];
    int a_pipe_out[2];
    pipe(a_pipe_in);
    pipe(a_pipe_out);
    
    if (fork() == 0) {
        close(a_pipe_in[1]);
        close(a_pipe_out[0]);
        processA(a_pipe_in[0], a_pipe_out[1]);
        exit(0);
    } 

    int a, b;
    pipe(pipe_for_son);
    char c = 'c';
    write(pipe_for_son[1], &c, sizeof(char));

    while (scanf("%d %d", &a, &b) == 2) {
        // son
        int pid;
        while((pid = fork()) == -1){
            wait(NULL);
        }
        if (!pid) {
            close(a_pipe_out[1]);
            close(a_pipe_in[0]);

            son(a_pipe_in[1], a_pipe_out[0], pipe_for_son[0], pipe_for_son[1], a, b); 
            exit(0);
        } 
    }

    close(pipe_for_son[0]);
    close(pipe_for_son[1]);
    close(a_pipe_in[0]);
    close(a_pipe_out[1]);
    close(a_pipe_in[1]);
    close(a_pipe_out[0]);
 
    while (wait(NULL) != -1);
    return 0;
}