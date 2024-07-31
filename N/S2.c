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
seq(Command *c) {
}

int 
simple(Command *c) {
    return 0;
}

int 
redirect(Command *c) {
    return 0;
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