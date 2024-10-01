#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


int main (int argc, char** argv) {

    if (argc > 3) {
        errno = E2BIG;
        perror("Error reading arguments");
        exit(EXIT_FAILURE);
    }
    if (argc < 3) {
        errno = EINVAL;
        perror("Error reading arguments");
        exit(EXIT_FAILURE);
    }

    char buffer[100];

    int fd1 = open(argv[1], O_RDONLY);
    if (fd1 == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    remove(argv[2]);
    int fd2 = open(argv[2], O_WRONLY | O_CREAT, 0664);
    if (fd2 == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }


    size_t bytes_read;
    size_t bytes_written;
    do {

        bytes_read = read(fd1, buffer, sizeof(buffer));

        if (bytes_read == -1) {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }

        bytes_written = write(fd2, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            perror("Error writing file");
            exit(EXIT_FAILURE);
        }

    } while (bytes_read != 0);

    do {

    } while (bytes_written );

    close(fd1);
    close(fd2);

    return 0;
}