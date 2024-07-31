#include <stdio.h>
#include <stdlib.h>

double* read_array(int* size) {
    scanf("%d", size);
    double* array = (double*)malloc(*size * sizeof(double));
    for (int i = 0; i < *size; i++) {
        scanf("%lf", &array[i]);
    }
    return array;
}

void print_array(double* array, int size) {
    for (int i = 0; i < size; i++) {
        printf("%.1lf ", array[i]);
    }
    printf("\n");
}

void swap(double* a, double* b) {
    double tmp = *a;
    *a = *b;
    *b = tmp;
}

int find_first_negative(double* array, int size) {
    for (int i = 0; i < size; i++) {
        if (array[i] < 0) {
            return i;
        }
    }
    return -1;
}

int find_last_positive(double* array, int size) {
    for (int i = size - 1; i >= 0; i--) {
        if (array[i] > 0) {
            return i;
        }
    }
    return -1;
}

int main(void) {
    int size1, size2;
    double* array1 = read_array(&size1);
    double* array2 = read_array(&size2);
    
    int index1 = find_first_negative(array1, size1);
    int index2 = find_last_positive(array2, size2);
    
    if (index1 != -1 && index2 != -1) {
        swap(&array1[index1], &array2[index2]);
    }
    
    print_array(array1, size1);
    print_array(array2, size2);
    free(array1);
    free(array2);
    
    return 0;
}