#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    int fd = open(argv[1], O_RDWR);
    char buffer[2];
    ssize_t n = read(fd, buffer, 2);
    if (n == 2) {
        buffer[1] = buffer[0];
        lseek(fd, 0, SEEK_SET);
        write(fd, buffer, 2);
    }
    close(fd);
    return 0;
}
