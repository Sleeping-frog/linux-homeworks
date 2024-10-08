#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>


int main (int argc, char** argv) {
    if (argc == 1) {
        errno = EINVAL;
        perror("Error reading arguments");
        exit(EXIT_FAILURE);
    }

    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 6*2*2*2*2*2*2 + 4*2*2*2 + 4);
    if (fd == -1) {
        perror("Error openning file");
        exit(EXIT_FAILURE);
    }

    if (write(fd, "first line\n", 11) == -1) {
        perror("Error writing file");
        exit(EXIT_FAILURE);
    }

    int new_fd = dup(fd);
    if (new_fd == -1) {
        perror("Error copying file descriptor");
        exit(EXIT_FAILURE);
    }
    if (write(fd, "second line\n", 12) == -1) {
        perror("Error writing file");
        exit(EXIT_FAILURE);
    }

    if (close(fd) == -1) {
        perror("Error closing file");
        exit(EXIT_FAILURE);
    }

    if (close(new_fd) == -1) {
        perror("Error closing file");
        exit(EXIT_FAILURE);
    }
}