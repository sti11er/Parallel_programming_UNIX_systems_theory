#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

#define EOF (-1)
#define SIZE 4096
#define F_SIZE 32

//byte reading 'n writing
int
b_read(int fd)
{
    char x;
    read(fd, &x, 1);
    if (lseek(fd, 0, SEEK_CUR) == F_SIZE) {
        lseek(fd, 0, SEEK_SET);
    }
    return x;
}

int
b_write(int fd, int x)
{
    write(fd, &x, 1);
    if (lseek(fd, 0, SEEK_CUR) == F_SIZE) {
        lseek(fd, 0, SEEK_SET);
    }

    return x;
}

int
main(void)
{
    char path[] = {"K6XXXXXX"};
    int fd_write = mkstemp(path);
    int fd_read = open(path, O_RDONLY);
    
    unlink(path);
    char c;
    int srav;
    
    if (fork() == 0){
        //son
        while (read(0,&c,1) == 1){
            srav = lseek(fd_read, 0, SEEK_CUR) - lseek(fd_write, 0, SEEK_CUR);
            if ((srav = 1) || (srav == (F_SIZE - 1))){
                while((lseek(fd_write, 0, SEEK_CUR) == lseek(fd_read, 0, SEEK_CUR) - 1) ||
                        ((lseek(fd_write, 0, SEEK_CUR) - lseek(fd_read, 0, SEEK_CUR)) == (F_SIZE - 1)) ) {
                    usleep(1000);
                }
            }
            //ready for write
            b_write(fd_write, c);
           
        }
        exit(0);
    } else {
        //father
        while((waitpid(-1, NULL, WNOHANG) == 0) || 
                ( (srav = (lseek(fd_read, 0, SEEK_CUR) - lseek(fd_write, 0, SEEK_CUR))) != 0) ) {

            while((srav = lseek(fd_read, 0, SEEK_CUR) - lseek(fd_write, 0, SEEK_CUR)) == 0){
                if (waitpid(-1, NULL, WNOHANG) != 0) {return 0;}  
                usleep(1000);
            }
            c = b_read(fd_read);
            write(1,&c, 1);       
        }
    }    
    return 0;
}