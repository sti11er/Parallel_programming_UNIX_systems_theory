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
main(void) {
    return 0;
}
