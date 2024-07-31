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

typedef struct state_t {
    int pid;
    int status;
} state_t;

state_t
run_command(Command *);

state_t 
seq1(Command *c) {
    state_t state; int status;
    for (int i = 0; i < c->seq_size; ++i) {
        switch (c->seq_operations[i]) {
            case OP_SEQ: {
                state = run_command(c->seq_commands + i);
                if (state.status == -1) {
                    waitpid(state.pid, &status, 0);
                    state.status = ((WEXITSTATUS(status) == 0) && WIFEXITED(status));
                }
                break;
            }
            case OP_BACKGROUND: {
                state = run_command(c->seq_commands + i);
                state.status = 1;
                break;
            }
            default:
                break;
            }
    }

    // while (wait(NULL) != -1);
    return state;
}

state_t 
seq2(Command *c) {
    state_t state; int status;
    state = run_command(c->seq_commands);
    if (state.status == -1) {
        waitpid(state.pid, &status, 0);
        state.status = ((WEXITSTATUS(status) == 0) && WIFEXITED(status));
    }

    for (int i = 1; i < c->seq_size; ++i) {
        switch (c->seq_operations[i-1]) {
            case OP_CONJUNCT: {
                if (state.status == 1) {
                    state = run_command(c->seq_commands + i);
                    if (state.status == -1) {
                        waitpid(state.pid, &status, 0);
                        state.status = ((WEXITSTATUS(status) == 0) && WIFEXITED(status));
                    }
                }
                break;
            }
            case OP_DISJUNCT: {
                if (state.status == 0) {
                    state = run_command(c->seq_commands + i);
                    if (state.status == -1) {
                        waitpid(state.pid, &status, 0);
                        state.status = ((WEXITSTATUS(status) == 0) && WIFEXITED(status));
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


state_t
pipeline(Command *);

int 
simple(Command *c) {
    int pid;
    if ((pid = fork()) == 0) {
        execvp(c->argv[0], c->argv);
        exit(1);
    }
    
    return pid;
}

state_t
redirect(Command * c) {
    state_t state;
    int buffer = 0, fd, mode = 0;
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
    state = run_command(c->rd_command);
    dup2(buffer, mode);
    return state;
}

state_t
run_command(Command *c) {
    state_t state;
    switch (c->kind) {
        case KIND_SIMPLE: {
            state.pid = simple(c);
            state.status = -1;
            break;
        }
        case KIND_SEQ1: {
            state = seq1(c);
            break;
        }
        case KIND_SEQ2: {
            state = seq2(c);
            break;
        }
        case KIND_PIPELINE: {
            state = pipeline(c);
            break;
        }
        case KIND_REDIRECT: {
            state = redirect(c);
            break;
        }         
        default: {
            break;
        }   
    }
    return state;
}

state_t
pipeline(Command * c) {
    int fd[2], prev = 0,
    init_in = dup(0), init_out = dup(1);
    state_t state;

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

        state = run_command(c->pipeline_commands + i);
        prev = fd[0];
    }
    dup2(init_in, 0);
    if (prev == 0) {
        close(prev);
    }
    return state;
}


int 
main(void) {
    return 0;
}


// (echo 2 && ./a) || echo 1
// (echo 2; ./a) || echo 1
// (echo 2; ./a &) || echo 1
// (echo 2 && ./a) || (./a || echo 5) || echo 1