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
    return 0;
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
    return 0;
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
main(void) {
    return 0;
}
