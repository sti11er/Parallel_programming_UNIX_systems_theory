#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>

enum {
    KIND_SIMPLE,
    KIND_PIPELINE,
    KIND_SEQ1, 
    KIND_SEQ2, 
    OP_SEQ, 
    KIND_REDIRECT, 
    RD_OUTPUT, 
    RD_INPUT, 
    OP_CONJUNCT, 
    OP_BACKGROUND,
    RD_APPEND, 
    OP_DISJUNCT, 
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
seq(Command *c) {
    int pid = 0, status = 0, fulfilled = 1;
    for (int i = 0; i < c->seq_size; ++i) {
        switch (c->seq_operations[i]) {
            case OP_SEQ: {
                if (fulfilled == 1) {
                    pid = run_command(c->seq_commands + i);
                    waitpid(pid, &status, 0);
                }
                else {
                    fulfilled = 1;
                }
                break;
            }
            case OP_BACKGROUND: {
                if (fulfilled == 1) {
                    pid = run_command(c->seq_commands + i);
                }
                else {
                    fulfilled = 1;
                }
                break;
            }
            case OP_CONJUNCT: {
                if (fulfilled == 1) {
                    pid = run_command(c->seq_commands + i);
                    waitpid(pid, &status, 0);
                }
                fulfilled = ((WEXITSTATUS(status) == 0) && WIFEXITED(status));
                break;
            }
            case OP_DISJUNCT: {
                if (fulfilled == 0) {
                    pid = run_command(c->seq_commands + i);
                    waitpid(pid, &status, 0);
                }
                fulfilled = ((WEXITSTATUS(status) == 0) && WIFEXITED(status));
                break;
            }
            default:
                break;
            }
    }

    while (wait(NULL) != -1);
    return pid;
}

int 
simple(Command *c) {
    int pid;
    if ((pid = fork()) == 0) {
        execvp(c->argv[0], c->argv);
        exit(1);
    }
    return pid;
}

int 
redirect(Command *c) {
    int pid = 0, 
    fd, buf = 0, mode = 0;
    switch (c->rd_mode) {
        case RD_INPUT: {
            buf = dup(0);
            fd = open(c->rd_path, O_RDONLY);
            dup2(fd, 0);
            mode = 0;
            close(fd);
            break;
        }
        case RD_OUTPUT: {
            buf = dup(1);
            fd = open(c->rd_path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            dup2(fd, 1);
            mode = 1;
            close(fd);
            break;
        }
        case RD_APPEND: {
            buf = dup(1);
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

    pid = run_command(c->rd_command);
    dup2(buf, mode);
    return pid;
}

int 
pipeline(Command *c) {
    int fd[2], buf = 0, 
    init_in = dup(0), init_out = dup(1), 
    pid = 0;

    for (int i = 0; i < c->pipeline_size; ++i) {
        if (i != 0) {
            dup2(buf, 0);
            close(buf);
        }
        if (i == (c->pipeline_size - 1)) {
            dup2(init_out, 1);
        }
        else {
            pipe(fd);
            dup2(fd[1], 1);
            close(fd[1]);
        }

        pid = run_command(c->pipeline_commands + i);
        buf = fd[0];
    }

    dup2(init_in, 0);
    if (buf == 0) {
        close(buf);
    }
    return pid;
}

int
run_command(Command *c) {
    int pid = 0;
    switch (c->kind) {
        case KIND_SIMPLE: {
            pid = simple(c);
            break;
        }
        case KIND_SEQ1: {
            pid = seq(c);
            break;
        }
        case KIND_SEQ2: {
            pid = seq(c);
            break;
        }
        case KIND_PIPELINE: {
            pid = pipeline(c);
            break;
        }
        case KIND_REDIRECT: {
            pid = redirect(c);
            break;
        }         
        default: {
            break;
        }   
    }
    return pid;
}

int 
main (void) {
    // ls | grep .c | grep S
    Command c = {
        .kind = KIND_PIPELINE

    };

    pipeline(c);
    return 0;
} 
