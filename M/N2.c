
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
son_func(char *argv0, int type, unsigned long long N) {
    key_t key = ftok(argv0, 1);
    int semid = semget(key, 2, 0); // идентификатор массива семафоров

    // Семафор, который сын увеличивает после печати числа
    int my_sem = type; 
    // Семафор, который сын уменьшает перед печати числа
    int other_sem = 1 - type; 
    // Определяем структуру для операций над семафорами
    struct sembuf op;
    op.sem_flg = 0; 

    for (unsigned long long i = type; i < N; i += 2) {
        op.sem_num = my_sem; 
        op.sem_op = -1; 
        semop(semid, &op, 1);
        // Печатаем число
        printf("%llu\n", i);
        fflush(stdout);
        // Отпускаем семафор для другого сына
        op.sem_num = other_sem;
        op.sem_op = 1;
        semop(semid, &op, 1);
    }
    exit(0);
}

char *saved;
void sigint_handler(int signum) {
    key_t key = ftok(saved, 1);
    int sem_id = semget(key, 2, 0);
    semctl(sem_id, 0, IPC_RMID); // удаляем ресурс массив семафоров
    exit(0);
}

int 
main(int argc, char **argv) {
    saved = argv[0];
    signal(SIGINT, sigint_handler);
    
    key_t key = ftok(argv[0], 1);
    // создаем ресурс
    int sem_id = semget(key, 2, IPC_CREAT | 0666); // идентификатор массива семафоров

    union semun arg;
    unsigned short vals[2] = {1,0};
    arg.array = vals;

    semctl(sem_id, 0, SETALL, arg);

    unsigned long long N;
    scanf("%llu", &N);
    for (int i=0; i<2; i++) {
        if (fork() == 0) {
            son_func(argv[0], i, N);
        }
    }

    while (wait(NULL) != -1); 
    semctl(sem_id, 0, IPC_RMID); // удаляем ресурс массив семафоров        
    return 0;
}