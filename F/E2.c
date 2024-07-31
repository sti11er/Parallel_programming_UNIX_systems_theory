#include <stdio.h>

enum { MAX = 100 };
void multiply(double mat1[][MAX], double mat2[][MAX], 
            double res[][MAX], int r1, int c1, int r2, int c2){
    for (int i = 0; i < r1; i++){
        for (int j = 0; j < c2; j++){
            res[i][j] = 0;
            for (int k = 0; k < c1; k++){
                res[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
}

int main(void) {
    double matrix1[MAX][MAX], matrix2[MAX][MAX], res[MAX][MAX];
    int r1, c1, r2, c2;

    scanf("%d %d", &r1, &c1);
    for (int i = 0; i < r1; i++){
        for (int j = 0; j < c1; j++){
            scanf("%lf", &matrix1[i][j]);
        }
    }

    scanf("%d %d", &r2, &c2);
    for (int i = 0; i < r2; i++){
        for (int j = 0; j < c2; j++){
            scanf("%lf", &matrix2[i][j]);
        }
    }

    multiply(matrix1, matrix2, res, r1, c1, r2, c2);
    for (int i = 0; i < r1; i++){
        for (int j = 0; j < c2; j++){
            printf("%.1lf ", res[i][j]);
        }
        printf("\n");
    }

    return 0;
}
