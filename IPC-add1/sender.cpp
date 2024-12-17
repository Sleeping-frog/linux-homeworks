#include <iostream>
#include <stdlib.h>
#include <signal.h>

int main (int argc, char** argv) {
	if (argc != 3) {
		std::cout << "wrong arguments\n";
		exit(EXIT_FAILURE);
	}
	kill(atoi(argv[2]), atoi(argv[1]));
}
