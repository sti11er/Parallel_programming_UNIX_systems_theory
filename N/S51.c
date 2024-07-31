#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

enum {
    KIND_SIMPLE,
    KIND_PIPELINE,
    KIND_SEQ1, 
    KIND_SEQ2, 
    KIND_REDIRECT, 
    OP_SEQ, 
    OP_CONJUNCT, 
    OP_BACKGROUND, 
    OP_DISJUNCT, 
    RD_OUTPUT, 
    RD_INPUT, 
    RD_APPEND,
};

typedef struct Command {
    int kind;
    union {
        struct {
            int argc;
            char ** argv;
        };
        struct {
            int pipeline_size;
            struct Command * pipeline_commands;
        };
        struct {
            int seq_size;
            struct Command * seq_commands;
            int * seq_operations;
        };
        struct {
            int rd_mode;
            char * rd_path;
            struct Command * rd_command;
        };
    };
} Command;


int
run_command(Command *);

int 
seq1(Command *c) {
    int state, status;
    for (int i = 0; i < c->seq_size; ++i) {
        switch (c->seq_operations[i]) {
            case OP_SEQ: {
                state = run_command(c->seq_commands + i);
                if (state == -1) {
                    wait(NULL);
                    state = ((WEXITSTATUS(status) == 0) && WIFEXITED(status));
                }
                break;
            }
            case OP_BACKGROUND: {
                state = 0;
                run_command(c->seq_commands + i);
                break;
            }
            default:
                break;
            }
    }

    // while (wait(NULL) != -1);
    return state;
}

int 
seq2(Command *c) {
    int status, state;

    state = run_command(c->seq_commands);
    if (state == -1) {
        wait(NULL);
        state = ((WEXITSTATUS(status) == 0) && WIFEXITED(status));
    }

    for (int i = 1; i < c->seq_size; ++i) {
        switch (c->seq_operations[i-1]) {
            case OP_CONJUNCT: {
                if (state == 1) {
                    state = run_command(c->seq_commands);
                    if (state == -1) {
                        wait(NULL);
                        state = ((WEXITSTATUS(status) == 0) && WIFEXITED(status));
                        printf("./a %d\n", state);
                    }
                }
                break;
            }
            case OP_DISJUNCT: {
                if (state == 0) {
                    state = run_command(c->seq_commands);
                    if (state == -1) {
                        wait(NULL);
                        state = ((WEXITSTATUS(status) == 0) && WIFEXITED(status));
                    }
                }
                break;
            }
            default: {
                break;
            }
        }
    }
    // while (wait(NULL) != -1);
    return state;
}


int
pipeline(Command *);

void 
simple(Command *c) {
    int pid, status;
    if ((pid = fork()) == 0) {
        execvp(c->argv[0], c->argv);
        exit(1);
    }
    return;
}

int
redirect(Command * c) {
    int buffer = 0, status = 0, fd, mode = 0;
    switch (c->rd_mode) {
        case RD_INPUT: {
            buffer = dup(0);
            fd = open(c->rd_path, O_RDONLY);
            dup2(fd, 0);
            mode = 0;
            close(fd);
            break;
        }
        case RD_OUTPUT: {
            buffer = dup(1);
            fd = open(c->rd_path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            dup2(fd, 1);
            mode = 1;
            close(fd);
            break;
        }
        case RD_APPEND: {
            buffer = dup(1);
            fd = open(c->rd_path, O_WRONLY | O_CREAT | O_APPEND, 0666);
            dup2(fd, 1);
            mode = 1;
            close(fd);
            break;
        }
        default: {
            break;
        }
    }
    status = run_command(c->rd_command);
    dup2(buffer, mode);
    return status;
}

int
run_command(Command *c) {
    int status;
    switch (c->kind) {
        case KIND_SIMPLE: {
            simple(c);
            status = -1;
            break;
        }
        case KIND_SEQ1: {
            status = seq1(c);
            break;
        }
        case KIND_SEQ2: {
            status = seq2(c);
            break;
        }
        case KIND_PIPELINE: {
            status = pipeline(c);
            break;
        }
        case KIND_REDIRECT: {
            status = redirect(c);
            break;
        }         
        default: {
            break;
        }   
    }
    return status;
}

int
pipeline(Command * c) {
    int fd[2], prev,
    init_in = dup(0), init_out = dup(1), status;

    for (int i = 0; i < c->pipeline_size; ++i) {
        if (i != 0) {
            dup2(prev, 0);
            close(prev);
        }

        if (i == c->pipeline_size-1) {
            dup2(init_out, 1);
        } else {
            pipe(fd);
            dup2(fd[1], 1);
            close(fd[1]);
        }

        status = run_command(c->pipeline_commands + i);
        prev = fd[0];
    }
    dup2(init_in, 0);
    if (prev == 0) {
        close(prev);
    }
    return status;
}


int 
main(void) {
    // command "(echo 2 && ./a) || echo 1"
    // 2 1
    Command a = {
        .kind = KIND_SIMPLE,
        .argc = 2,
        .argv = (char *[]) {"echo", "1", 0},
    };
    Command a_1 = {
        .kind = KIND_PIPELINE,
        .pipeline_size = 1,
        .pipeline_commands = &a,
    };
    Command b = {
        .kind = KIND_SIMPLE,
        .argc = 2,
        .argv = (char *[]) {"echo", "2", 0},
    };
    Command b_1 = {
        .kind = KIND_PIPELINE,
        .pipeline_size = 1,
        .pipeline_commands = &b,
    };
    Command c6_2_1_2_1 = {
        .kind = KIND_SIMPLE,
        .argc = 1,
        .argv = (char *[]) {"./a", 0},
    };
    Command c6_2_1_2 = {
        .kind = KIND_PIPELINE,
        .pipeline_size = 1,
        .pipeline_commands = &c6_2_1_2_1,
    };
    Command c6_2_1 = {
        .kind = KIND_SEQ2,
        .seq_size = 2,
        .seq_commands = (Command []) {b_1, c6_2_1_2},
        .seq_operations = (int []) {OP_CONJUNCT},
    };
    Command c6_2 = {
        .kind = KIND_PIPELINE,
        .pipeline_size = 1,
        .pipeline_commands = &c6_2_1,   
    };
    Command c6 = {
        .kind = KIND_SEQ2,
        .seq_size = 2,
        .seq_commands = (Command []) {c6_2, a_1},
        .seq_operations = (int []) {OP_DISJUNCT},
    };
    run_command(&c6);

    // (echo 2; ./a) || echo 1
    // 2 1
    Command c6_2_2 = {
        .kind = KIND_SEQ1,
        .seq_size = 2,
        .seq_commands = (Command []) {b_1, c6_2_1_2},
        .seq_operations = (int []) {OP_SEQ},
    };
    Command c6_3 = {
        .kind = KIND_PIPELINE,
        .pipeline_size = 1,
        .pipeline_commands = &c6_2_2,   
    };
    Command c7 = {
        .kind = KIND_SEQ2,
        .seq_size = 2,
        .seq_commands = (Command []) {c6_3, a_1},
        .seq_operations = (int []) {OP_DISJUNCT},
    };
    // run_command(&c7);

    // (echo 2 && ./a) || (./a || echo 2) || echo 1
    // 2 2 
    Command adb = {
        .kind = KIND_SEQ2,
        .seq_size = 2,
        .seq_commands = (Command []) {c6_2_1_2, b_1},
        .seq_operations = (int []) {OP_DISJUNCT},
    };
    Command asdfsadf = {
        .kind = KIND_PIPELINE,
        .pipeline_size = 1,
        .pipeline_commands = &adb,   
    };
    Command c8 = {
        .kind = KIND_SEQ2,
        .seq_size = 2,
        .seq_commands = (Command []) {c6_3, asdfsadf, a_1},
        .seq_operations = (int []) {OP_DISJUNCT, OP_DISJUNCT},
    };
    // run_command(&c8);

    // (echo 2; ./a &) || echo 1
    // 2
    Command fff = {
        .kind = KIND_SEQ1,
        .seq_size = 2,
        .seq_commands = (Command []) {b_1, c6_2_1_2},
        .seq_operations = (int []) {OP_SEQ, OP_BACKGROUND},
    };
    Command c9 = {
        .kind = KIND_SEQ2,
        .seq_size = 2,
        .seq_commands = (Command []) {fff, a_1},
        .seq_operations = (int []) {OP_DISJUNCT},
    };
    // run_command(&c9);

    return 0;
}


// (echo 2 && ./a) || echo 1
// (echo 2; ./a) || echo 1
// (echo 2; ./a &) || echo 1
// (echo 2 && ./a) || (./a || echo 5) || echo 1