#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int 
main(void) {
    char *tmpname = "tmpXXXXXXX";
    int fd = mkstemp(tmpname);

    int num, n;
    scanf("%d", &n);

    num = 0;
    write(fd, &num, sizeof(int));
    


    return 0;
}
