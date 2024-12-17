#include <iostream>
#include <signal.h>
#include <unistd.h>

int usr2_counter = 0;

void usr2_handler(int signum) {
	++usr2_counter;
}

void int_handler(int signum) {
	std::cout << "Got " << usr2_counter << " SIGUSR2 signals.\n";
	exit(0);
}

int main() {
	struct sigaction act1;
	act1.sa_handler = SIG_IGN;
	sigaction(SIGUSR1, &act1, nullptr);
	struct sigaction act2;
        act2.sa_handler = &usr2_handler;
        sigaction(SIGUSR2, &act2, nullptr);
	struct sigaction act3;
        act3.sa_handler = &int_handler;
        sigaction(SIGINT, &act3, nullptr);
	while (true) {
		std::cout << "I am still alive!!" << std::endl;
		sleep(5);
	}
}
