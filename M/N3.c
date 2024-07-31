#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <signal.h>

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void 
son_func(char *argv0, int type, int N) {
    key_t key = ftok(argv0, 1);
    // идентификатор массива семафоров
    int semid = semget(key, 2, 0);
    // идентификатор раз памяти
    int shmid = shmget(key, 10000000*N, 0);
    // прикрепляем сегмент к адресному пространству процесса
    int *num = (int *)shmat(shmid, (void *)0, 0);

    // Семафор, который сын увеличивает после печати числа
    int my_sem = type; 
    // Семафор, который сын уменьшает перед печати числа
    int other_sem = 1 - type; 
    struct sembuf op;
    op.sem_flg = 0; 

    int start = type ? (N-1) : 0;
    int end = type ? (N-1) : 0;
    int step = type ? -1 : 1;

    for (int i = start; (type ? (i >= end):(i < end)); i+=step) {
        op.sem_num = my_sem; 
        op.sem_op = -1; 
        semop(semid, &op, 1);
        num[i]++;
        op.sem_num = other_sem;
        op.sem_op = 1;
        semop(semid, &op, 1);
    }

    shmdt(num);
    exit(0);
}

char *saved; int N;
void sigint_handler(int signum) {
    key_t key = ftok(saved, 1);
    int semid = semget(key, 2, 0);
    int shmid = shmget(key, 10000000*N, 0);

    semctl(semid, 0, IPC_RMID);
    shmctl(shmid, IPC_RMID, NULL); 
    exit(0);
}

int 
main(int argc, char **argv) {
    saved = argv[0];
    signal(SIGINT, sigint_handler);
    scanf("%d", &N);

    key_t key = ftok(argv[0], 1);

    // идентификатор массива семафоров
    int semid = semget(key, 2, IPC_CREAT | 0666);    
    // идентификатор разд памяти
    int shmid = shmget(key, 10000000*N, IPC_CREAT | 0666);

    // прикрепляем сегмент к адресному пространству процесса
    int *num = (int *)shmat(shmid, (void *)0, 0);
    for (int i=0; i<N; i++) {
        num[i] = i;
    }

    unsigned short arg[2] = {1,0};
    semctl(semid, 0, SETALL, arg);

    for (int i=0; i<2; i++) {
        if (fork() == 0) {
            son_func(argv[0], i, N);
        }
    }

    while(wait(NULL) != -1);

    for (int i=0; i<N; i++) {
        printf("%d\n", num[i]);
    }

    shmdt(num);
    shmctl(shmid, IPC_RMID, NULL); 
    semctl(semid, 0, IPC_RMID);
    
    return 0;
}
