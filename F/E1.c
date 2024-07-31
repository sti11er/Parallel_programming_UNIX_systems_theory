#include <stdio.h>

double* find_max(double* arr, int size) {
    if (size == 0) {
        return NULL;
    }
    double* max = arr;
    for (int i = 1; i < size; i++) {
        if (arr[i] > *max) {
            max = &arr[i];
        }
    }
    return max;
}

void swap(double* a, double* b) {
    double tmp = *a;
    *a = *b;
    *b = tmp;
}

void sort_array(double* arr, int size) {
    for (int i = 0; i < size - 1; i++) {
        double* max = find_max(&arr[i], size - i);
        swap(max, &arr[i]);
    }
}

int main(void) {
    int n;
    scanf("%d", &n);
    double arr[n];
    for (int i = 0; i < n; i++) {
        scanf("%lf", &arr[i]);
    }

    if (n >= 18) {
        double* max_element = find_max(&arr[4], 14);
        if (max_element != NULL) {
            printf("%.1lf\n", *max_element);
        }
    }

    sort_array(arr, n);

    for (int i = 0; i < n; i++) {
        printf("%.1lf ", arr[i]);
    }
    printf("\n");
    return 0;
}
