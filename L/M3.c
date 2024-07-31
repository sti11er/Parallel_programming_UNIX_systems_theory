#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>

time_t last_print_time;
void 
handler(int signum) {
    time_t current_time = time(NULL);
    int seconds = current_time - last_print_time;
    printf("%d\n", 5 - seconds);
}

int 
main(void) {
    unsigned long long num;
    last_print_time = time(NULL);

    signal(SIGINT, handler);
    scanf("%llu", &num);
    
    while (1) {
        if (num == 0) { break; }
        num--;
        
        time_t current_time = time(NULL);
        if ((current_time - last_print_time) == 5) {
            last_print_time = current_time;
            printf("%llu\n", num);
        }
    }
    
    return 0;
}
