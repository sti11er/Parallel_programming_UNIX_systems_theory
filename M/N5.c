#include <stdio.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define SHEAR1 10000000000
#define SHEAR2 20000000000
// #define TIMEOUT 5

struct msbufer {
    long mtype;
    char mtext[6];
};



// очередь
// увеличить число (сервер) - 1
// вернуть текущее число (сервер) - 2 
// завершиться (сервер) - 3
// создать сервер (регистратор)- 4 
// завершиться (регистратор) - 5

void 
server(char *argv, int id) {
    int number = 0;
    struct msbufer msg; 
    key_t key = ftok(argv, 1);
    int msgid = msgget(key, 0);

    pid_t pid;

    while ((pid = fork()) == -1) {
        usleep(10);
    }

    // процесс-сервер
    if (pid == 0) {
        while (1) {
            msgrcv(msgid, &msg, 6, id, 0);
            if (msg.mtext[0] == '1') {
                number++;
            } else if (msg.mtext[0] == '2') {
                msg.mtype = (id+SHEAR1);
                sprintf(msg.mtext, "%d", number);
                msgsnd(msgid, &msg, 6, 0);
            } else if (msg.mtext[0] == '3') {
                exit(0);
            }
        }
    }
}

void  
reg(char *argv) {
    struct msbufer msg; 
    key_t key = ftok(argv, 1);
    int msgid = msgget(key, 0);

    if (fork() == 0) {
        while (1) {
            msgrcv(msgid, &msg, 6, 1, 0);
            // if (strcmp(msg.mtext, "-1") == 0) {
            //     printf("jjj\n");
            //     exit(0);
            // }
 
            int id = atoi(msg.mtext);
            server(argv, id);
            // if (server(argv, id) == 1) {
            //     msg.mtype = id+SHEAR2;
            //     strcpy(msg.mtext, "nice");
            //     msgsnd(msgid, &msg, 6, 0);
            // }
        }
    }
}

// void 
// client_handler(int signum) {
//     exit(1);
// }

void 
client(char *argv, int N, int id) {
    struct msbufer msg; 
    key_t key = ftok(argv, 1);
    int msgid = msgget(key, 0);

    pid_t pid;
    while ((pid = fork()) == -1) {
        usleep(10);
    }

    // процесс-клиент
    if (pid == 0) {
        // signal(SIGALRM, client_handler);
        msg.mtype = 1;
        sprintf(msg.mtext, "%d", id);
        msgsnd(msgid, &msg, 6, 0);

        printf("%d\n", id);

        msg.mtype = id;
        msg.mtext[0] = '1';
        for (int i=0; i<N; i++) {
            msgsnd(msgid, &msg, 6, 0);
        }
        msg.mtext[0] = '2';
        msgsnd(msgid, &msg, 6, 0);
        msgrcv(msgid, &msg, 6, (id+SHEAR1), 0);
        printf("%s\n", msg.mtext);
        fflush(stdout);

        alarm(0);
        msg.mtype = id;
        msg.mtext[0] = '3';
        msgsnd(msgid, &msg, 6, 0);
        exit(0);
    } 
}

char *params;
void 
handler(int signum) {
    key_t key = ftok(params, 1);
    int msgid = msgget(key, 0);

    msgctl(msgid, IPC_RMID, NULL);
    exit(0);
}

int 
main(int argc, char **argv) {
    int N;  
    key_t key = ftok(argv[0], 1);
    // очередь для обмена msg между серверном и клиентом
    int msgid = msgget(key, IPC_CREAT | 0666); 


    params = argv[0];
    // signal(SIGINT, handler);
    reg(argv[0]);

    int i=2;
    while (1) {
        if (scanf("%d", &N) == -1) { 
            // struct msbufer msg;
            // msg.mtype = 1;
            // char *str = "-1";
            // strcpy(msg.mtext, str);
            // msgsnd(msgid, &msg, 6, 0);
            break; 
        }
        fflush(stdin);
        client(argv[0], N, i);
        i++;
    }
    
    while (wait(NULL) != -1);

    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}