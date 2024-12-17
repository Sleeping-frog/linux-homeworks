#include <iostream>
#include <unistd.h>
#include <signal.h>

int main() {
	if (fork()) {
		while (true) {
			std::cout << "I'm parent and I'm alive." << std::endl;
			sleep(1);
		}
	}
	else {
		sleep(5);
		kill(getppid(), 9);
		std::cout << "Child here and I've killed my parent.\n";
	}
}
