#include <iostream>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>


void initialize(int argc, char** argv)
{
    if (close(0) == -1) {
        perror("Error redirecting input");
        exit(EXIT_FAILURE);
    }
    if (open(argv[1], O_RDONLY) == -1) {
        perror("Error openning file");
        exit(EXIT_FAILURE);
    }
}
 
int main(int argc, char** argv)
{
    if (argc == 1) {
        errno = EINVAL;
        perror("Error reading arguments");
        exit(EXIT_FAILURE);
    }

    initialize(argc, argv);
    
    std::string input;
    std::cin >> input;
    std::cout << input << '\n';
    
    std::string reversed = input;

    const std::size_t length = input.length();
    
    for (int i = 0; i < length; ++i) {
        reversed[length - i - 1] = input[i];
    }
    
    std::cout << reversed << '\n';
 
    return 0;
}
