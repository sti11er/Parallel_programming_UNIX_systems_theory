#include <stdio.h>
#include <stdarg.h>
#include <float.h>

enum Type {DOUBLE, INT};
void vmax(enum Type type, size_t n, ...) {
    va_list args;
    va_start(args, n);
    if (type == DOUBLE) {
        double max = va_arg(args, double);
        for (int i = 1; i < n; ++i) {
            double val = va_arg(args, double);
            if (val > max) { 
                max = val; 
            }
        }
        printf("%g\n", max);
    }
    else if (type == INT) {
        int max = va_arg(args, int);
        for (int i = 1; i < n; ++i) {
            int val = va_arg(args, int);
            if (val > max) { 
                max = val; 
            }
        }
        printf("%d\n", max);
    }

    va_end(args);
}

int main(void) {
    vmax(INT, 2, 1, 10);
    vmax(INT, 10, 10, -1, -100, 3, 3, 5, 6, 9, 0, 4, 10);
    vmax(INT, 3, 1, 10, 5);
    vmax(INT, 1, 10);
    vmax(DOUBLE, 4, 1.0, 2.0, 3.0, 100.0); 
    vmax(DOUBLE, 4, 100.0, 3.0, 2.0, 1.0);
    vmax(DOUBLE, 3, -1.0, 100.00, 100.0); 
    vmax(DOUBLE, 3, -1.0, -DBL_MAX, 100.0); 
}