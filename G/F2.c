#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int N; 
    scanf("%d", &N);
    int *array = malloc(N * sizeof(int)); 
    if (array == NULL) { 
        return 2;
    }
    for (int i = 0; i < N; i++) { 
        scanf("%d", &array[i]);
    }
    for (int i = N - 1; i >= 0; i--) { 
        printf("%d ", array[i]); 
    }
    printf("\n"); 
    free(array);
    return 0;
}
