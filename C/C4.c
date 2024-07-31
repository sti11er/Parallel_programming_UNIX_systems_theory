#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdarg.h>

static void output(const char buffer[])
{
    printf("%s", buffer);
    fflush(stdout);
}

enum { BUF_SIZE = 8 };

void print(const char format[], ...){
    static char buffer[BUF_SIZE]; 
    static int l = 0; 
    va_list args;
    va_start(args, format);

    for (int i = 0; (format[i] != '\0'); i++) {
        if ((l < (BUF_SIZE-1) && (format[i] == '\n'))) {
            output(buffer);
            buffer[++l] = '\n'; buffer[++l] = '\0';
            l = 0;
        }
        else if (l == (BUF_SIZE-1)) {
            output(buffer);
            buffer[++l] = '\0';
            l = 0;
            buffer[++l] = format[i];
        }
        else if (format[i]=='%') {
            buffer[l++] = (char) va_arg(args, int);
        }
        else {
            buffer[l++] = format[i];
        }
    }

    va_end(args);
}

static void
action0(void)
{
    print("123\n");
}
static const char * const responce0 = "123\n";

static void
action1(void)
{
    print("%+%\n", 'A', 'B');
}
static const char * const responce1 = "A+B\n";

static void
action2(void)
{
    print("12");
    print("34\n");
}
static const char * const responce2 = "1234\n";

static void
action3(void)
{
    print("12\n");
    print("34");
}
static const char *const responce3 = "12\n";

static char buf4[BUF_SIZE];
static void action4(void)
{
    print(&buf4[0]);
}
static const char * const responce4 = "";

static char buf5[BUF_SIZE + 1];
static void action5(void)
{
    print(&buf5[0]);
}
static const char * const responce5 = buf4;

static void action6(void)
{
    print("12");
    printf("$$"); fflush(stdout);
    print("34\n");
}
static const char * const responce6 = "$$1234\n";

static void action7(void)
{
    print("12\n34\n");
}
static const char * const responce7 = "12\n34\n";

static void action8(void)
{
    print("12\n3");
    printf("!!"); fflush(stdout);
    print("45\n");
}
static const char * const responce8 = "12\n!!345\n";

static void action9(void)
{
    print("1234\nABCDE");
    printf("@@"); fflush(stdout);
    print("tmp\n");
}
static const char * const responce9 = "1234\n@@ABCDEX\n";

static void action10(void)
{
    print("01234567890123456789");
    printf("**"); fflush(stdout);
}
static const char * const responce10 = "01234567890123**";

int
main(void)
{
    typedef struct {
        void (*action)(void);
        const char *responce;
    } Test;

    static Test tests[] = {
        { action0, responce0, },
        { action1, responce1, },
        { action2, responce2, },
        { action3, responce3, },
        { action4, responce4, },
        { action5, responce5, },
        { action6, responce6, },
        { action7, responce7, },
        { action8, responce8, },
        { action9, responce9, },
        { action10, responce10, },
    };

    {
        for (size_t i = 0; i < BUF_SIZE - 1; ++i) {
            buf4[i] = 'q';
        }
        for (size_t i = 0; i < BUF_SIZE; ++i) {
            buf5[i] = 'q';
        }
    }

    int ok = 1;
    for (size_t i = 0; i < sizeof tests / sizeof *tests; ++i) {
        int fd[2];
        if (pipe(fd) == -1) {
            perror("pipe");
            return 1;
        }
        if (fork() == 0) {
            dup2(fd[1], 1);
            close(fd[0]);
            close(fd[1]);
            tests[i].action();
            return 0;
        }
        close(fd[1]);

        FILE *in = fdopen(fd[0], "r");
        if (!in) {
            perror("fdopen-in");
            return 1;
        }

        static char buffer[1024];
        memset(&buffer[0], '\0', sizeof buffer);
        size_t overflow = 0;
        {
            size_t start = 0;
            while (1) {
                if (start >= sizeof buffer) {
                    overflow = 1;
                    break;
                } else if (!fgets(&buffer[start], sizeof buffer - start, in)) {
                    break;
                } else {
                    start += strlen(&buffer[start]);
                }
            }
        }
        if (overflow) {
            printf("TEST %lu FAILED: large output\n", i);
            ok = 0;
        } else if (strcmp(buffer, tests[i].responce) != 0) {
            printf("TEST %lu FAILED: wrong output (gotten len = %lu)\n", i, strlen(buffer));
            ok = 0;
        }

        fclose(in);
        close(fd[0]);
        int status;
        wait(&status);
        if (! WIFEXITED(status)) {
            perror("wait");
            return 1;
        }
    }

    if (ok) {
        printf("OK\n");
    }
}
