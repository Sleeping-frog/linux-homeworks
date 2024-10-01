#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>


int main (int argc, char** argv) {

    if (argc > 2) {
        errno = E2BIG;
        perror("Error reading arguments");
        exit(EXIT_FAILURE);
    }
    if (argc == 1) {
        errno = EINVAL;
        perror("Error reading arguments");
        exit(EXIT_FAILURE);
    }


    int fd = open(argv[1], O_WRONLY);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("Error getting file status");
        exit(EXIT_FAILURE);
    }

    size_t remaining_size = st.st_size;

    char buffer[100] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

    while (remaining_size >= 100) {
        if (write(fd, buffer, 100) == -1) {
            perror("Error erasing file");
            exit(EXIT_FAILURE);
        }
        remaining_size -= 100;
    }
    if (write(fd, buffer, remaining_size) == -1) {
        perror("Error erasing file");
        exit(EXIT_FAILURE);
    }

    close(fd); 

    if (remove(argv[1])) {
        perror("Error deleting file");
        exit(EXIT_FAILURE);
    }

    return 0;
}