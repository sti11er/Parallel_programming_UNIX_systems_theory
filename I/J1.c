#include <stdio.h>
#include <string.h>

int main(void) {
    char *program1_output_variants[] = {"A\nB\nB\nC\nC\n", "A\nB\nC\nB\nC\n"};
    char *program2_output_variants[] = {"1\n2\n2\n", "2\n1\n2\n", "2\n2\n1\n"};

    long long n;
    scanf("%lld", &n);
    if (n < 2 && n >= 0) {
        printf("%s", program1_output_variants[n]);
    } else {
        printf("UNKNOWN\n");
    }

    scanf("%lld", &n);
    if (n < 3 && n >= 0) {
        printf("%s", program2_output_variants[n]);
    } else {
        printf("UNKNOWN\n");
    }

    return 0;
}