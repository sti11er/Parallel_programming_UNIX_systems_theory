#include <stdio.h>

const int GETVAL = 0;
const int SETVAL = 1;

// typedef struct {
//     union Argument{
//         int value;
//         int *pvalue;
//     };
// } Argument;

union Argument{
    int value;
    int *pvalue;
};

typedef struct  {
    int id;
    int value;
} data_t;

data_t data[10000];

int command(int id, int cmd, union Argument arg) {
    if (id <= 0 || id > 10000) {
        return 1;
    }
    if (cmd != GETVAL && cmd != SETVAL) {
        return 2;
    }
    for (int i = 0; i < 10000; ++i) {
        if (data[i].id == id) {
            if (cmd == GETVAL) {
                *(arg.pvalue) = data[i].value;
            } else {
                data[i].value = arg.value;
            }
            return 0;
        }
    }
    return 1;
}

int main(void) {
    int i, id, value;

    for (int j = 0; j < 2; j++){
        scanf("%d %d %d", &i, &id, &value);
        data[i].id = id;
        data[i].value = value;
    }

    int d;
    scanf("%d", &d);

    union Argument arg;
    arg.pvalue = &value;

    if (command(d, GETVAL, arg) == 0) {
        arg.value = value + 1;
        command(d, SETVAL, arg);
        printf("%d\n", arg.value);
    } else {
        printf("NONE\n");
    }

    return 0;
}