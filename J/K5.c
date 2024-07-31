// #include <stdlib.h>
// #include <unistd.h>
// #include <fcntl.h>
// #include <sys/wait.h>

// #define SIZE 4096 

// void 
// copy_file(int src, int tmp) {
//     char buf[SIZE];
//     int line = 1; int n;
//     while ((n = read(src, buf, SIZE)) > 0) {
//         for (int i = 0; i < n; i++) { 
//             if (buf[i] == '\n') { 
//                 line++; 
//             }
//             if (line != 2) { 
//                 write(tmp, &buf[i], 1); 
//             }
//         }
//     }
// }

// void 
// write_in_file(int src, int tmp) {
//     char buf[180];
//     int n;
//     while ((n = read(tmp, buf, 180)) > 0) {
//         for (int i = 0; i < n; i++) { 
//             write(src, &buf[i], 1); 
//         }
//     }
// }

// int 
// main(int argc, char *argv[]) {
//     char tmpname[] = "tmpXXXXXX";
//     int tmp = mkstemp(tmpname);
//     unlink(tmpname);
//     char *filename = argv[1]; 
//     int src = open(filename, O_RDONLY); 
//     copy_file(src, tmp);
//     close(src);
 
//     lseek(tmp, 0, SEEK_SET);
//     close(src);
//     src = open(filename, O_WRONLY | O_TRUNC);
//     write_in_file(src, tmp);
    
//     close(src);
//     close(tmp);
//     return 0; 
// }

#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#define SIZE 4096
#define EOF (-1)

int my_getc(int fd){
    static int fd_p = -1;
    static char buf[SIZE] = {"\0"};
    static int capacity = 0;
    static int pos = 0;
    static int flag = 0;
 
    if (fd != fd_p) {
        fd_p = fd;
        capacity = 0;
        pos = 0;
        flag = 0;
    }
    if ((pos == capacity)) {
        if (flag){
            return -1;
        }
        capacity = read(fd_p, buf, SIZE);
        if ((capacity < SIZE)){
            flag = 1;
            if (capacity <= 0) {
                return -1;
            }
        }
        pos = 0;
    }
    
    return buf[pos++];
}

int my_putc(int fd, int c) 
{
    static int fd_p = -1;
    static char buf[SIZE] = {"0"};
    static int capacity = SIZE;
    static int pos = 0;

    if ((pos == capacity) || (fd_p != fd) || (c == -1)) {
        write(fd_p, buf, pos);
        pos = 0;
        fd_p = fd;
    }
    if (c == -1){
        return -1;
    }
    buf[pos++] = c;
    return c;    
}

int
main(int argc, char ** argv)
{
    char c;
    int cnt = 1;
    char path[] = {"K5XXXXXX"};
    int fd = mkstemp(path);
    int file = open(argv[1], O_RDONLY);
    unlink(path);
   
    while ((c = my_getc(file)) != EOF) {
        if (cnt != 2) { 
            my_putc(fd,c);
        }
        if (c == '\n') {
            cnt++;
        }
    }
    my_putc(fd, -1);
    lseek(fd, 0, SEEK_SET);
    close(file);
    file = open(argv[1], O_WRONLY | O_TRUNC);
    while ((c = my_getc(fd)) != EOF) {
        my_putc(file,c);
    }
    my_putc(file, -1);
    close(fd);
    return 0;
}