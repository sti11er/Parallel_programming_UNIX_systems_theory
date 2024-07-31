#include <stdio.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

struct msbufer1 {
    long mtype;
    char mtext[1];
};

struct msbufer2 {
    long mtype;
    char mtext[6];
};

void client(char *argv, int N) {
    struct msbufer1 msg1; 
    struct msbufer2 msg2; 
    key_t key1 = ftok(argv, 1);
    int msgid1 = msgget(key1, 0);
    key_t key2 = ftok(argv, 2);
    int msgid2 = msgget(key2, 0);

    // процесс-клиент
    if (fork() == 0) {
        msg1.mtype = 1;
        msg1.mtext[0] = '1';
        for (int i=0; i<N; i++) {
            msgsnd(msgid1, &msg1, 1, 0);
        }
        msg1.mtext[0] = '2';
        msgsnd(msgid1, &msg1, 1, 0);

        msgrcv(msgid2, &msg2, 6, 1, 0);
        printf("%s\n", msg2.mtext);

        msg1.mtext[0] = '3';
        msgsnd(msgid1, &msg1, 1, 0);
        exit(0);
    }
}

// процесс-сервер
// увеличить число - 1
// вернуть текущее число - 2 
// завершиться - 3
void server(char *argv) {
    int number = 0;
    struct msbufer1 msg1; 
    struct msbufer2 msg2; 
    key_t key1 = ftok(argv, 1);
    int msgid1 = msgget(key1, 0);
    key_t key2 = ftok(argv, 2);
    int msgid2 = msgget(key2, 0);

    // процесс-сервер
    if (fork() == 0) {
        while (1) {
            msgrcv(msgid1, &msg1, 1, 1, 0);
            if (msg1.mtext[0] == '1') {
                number++;
            } else if (msg1.mtext[0] == '2') {
                msg2.mtype = 1;
                sprintf(msg2.mtext, "%d", number);
                msgsnd(msgid2, &msg2, 6, 0);
            } else if (msg1.mtext[0] == '3') {
                exit(0);
            }
        }
    }
}

char *params;
void 
handler(int signum) {
    key_t key1 = ftok(params, 1);
    int msgid1 = msgget(key1, 0);
    key_t key2 = ftok(params, 2);
    int msgid2 = msgget(key2, 0);

    msgctl(msgid1, IPC_RMID, NULL);
    msgctl(msgid2, IPC_RMID, NULL);
    exit(0);
}

int 
main(int argc, char **argv) {
    int N;  
    key_t key1 = ftok(argv[0], 1);
    int msgid1 = msgget(key1, IPC_CREAT | 0666);
    key_t key2 = ftok(argv[0], 2);
    int msgid2 = msgget(key2, IPC_CREAT | 0666);

    params = argv[0];
    signal(SIGINT, handler);

    scanf("%d", &N);


    server(argv[0]);
    client(argv[0], N);

    while (wait(NULL) != -1);
    msgctl(msgid1, IPC_RMID, NULL);
    msgctl(msgid2, IPC_RMID, NULL);
    return 0;
}