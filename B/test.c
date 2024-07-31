#include <stdio.h>

int main(void) { 
    int label, state, flag;
    int fsm[100] = {0, 1, 2,
        0, 2, 1,
        2, 1, 0,};
    int cache[3];
    unsigned fsm_size = 3;

    char *ans = "YES";
    state = 0;
main_loop:
    while (1) {
        scanf("%d",&label);
        if (label == 0) {
            if (state != 1) {
                ans = "NO";
            }
            break;
        }
        
        flag = 1;
        // check cache
        if ((cache[0] == state) && (cache[1] == label)){
            state = cache[2];
            continue;
        } 

        for (size_t i = 0; i < (fsm_size*3); i+=3)
        {
            if (label != fsm[i+1]) {
                continue;
            }
            if (state == fsm[i]) {
                cache[0] = fsm[i];
                cache[1] = fsm[i+1];
                cache[2] = fsm[i+2];
      
                state = fsm[i+2];
                flag = 0;
                break; 
            }
        }
        if (flag) {
            ans = "NO";
        }
    }
    printf("%s", ans);
}