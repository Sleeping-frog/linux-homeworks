#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


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

    char buffer[100];

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    size_t bytes_read;
    do {

        bytes_read = read(fd, buffer, sizeof(buffer) - 1);

        if (bytes_read == -1) {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }

        buffer[bytes_read] = '\0';
        printf("%s", buffer);

    } while (bytes_read != 0);
    
    printf("\n");

    close(fd);  

    return 0;
}