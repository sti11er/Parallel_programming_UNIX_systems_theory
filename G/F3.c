#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int *arr = NULL;
    int size = 0;
    int capacity = 0;
    int num;

    while (scanf("%d", &num) == 1){
        if (size == capacity) {
            capacity = capacity == 0 ? 1 : capacity * 2;
            arr = realloc(arr, capacity*sizeof(int));
            if (arr == NULL) {
                return 1;
            }
        }
        arr[size] = num;
        size++;
    }
    for (int i = size-1; i >= 0; i--) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    free(arr);
    return 0;
}
