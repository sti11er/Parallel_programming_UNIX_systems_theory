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
    unsigned short  *array;
};

void 
son_func(char *argv0) {
    key_t key = ftok(argv0, 1);
    
    // открываем ресурсы
    semget(key, 2, 0); // идентификатор массива семафоров
    shmget(key, 1024, 0); // идентификатор разделяемой памяти
    msgget(key, 0); // идентификатор очереди сообщений

    exit(0);
}

char *saved;
void sigint_handler(int signum) {
    key_t key = ftok(saved, 1);

    int sem_id = semget(key, 2, 0);
    int shm_id = shmget(key, 1024, 0);
    int msg_id = msgget(key, 0);

    semctl(sem_id, 0, IPC_RMID); // удаляем ресурс массив семафоров
    shmctl(shm_id, IPC_RMID, NULL); // удаляем ресурс разделяемая память
    msgctl(msg_id, IPC_RMID, NULL); // удаляем ресурс очередь

    exit(0);
}

int 
main(int argc, char **argv) {
    saved = argv[0];
    signal(SIGINT, sigint_handler);

    key_t key = ftok(argv[0], 1);
    // создаем ресурсы
    int sem_id = semget(key, 2, IPC_CREAT | 0666); // идентификатор массива семафоров
    int shm_id = shmget(key, 1024, IPC_CREAT | 0666); // идентификатор разделяемой памяти
    int msg_id = msgget(key, IPC_CREAT | 0666); // идентификатор очереди сообщений

    // устанавливаем начальные значения семафоров в 0
    unsigned short sem_values[2] = {0, 0};
    semctl(sem_id, 0, SETALL, sem_values);

    if (fork() == 0) {
        son_func(argv[0]);
    } 

    wait(NULL); 
    
    semctl(sem_id, 0, IPC_RMID); // удаляем ресурс массив семафоров
    shmctl(shm_id, IPC_RMID, NULL); // удаляем ресурс разделяемая память
    msgctl(msg_id, IPC_RMID, NULL); // удаляем ресурс очередь
        
    return 0;
}
