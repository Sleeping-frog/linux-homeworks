#include <iostream>
#include <unistd.h>
#include <signal.h>

int prime(int num) {
	if (num == 1)
		return 2;
	int prime_counter = 1;
	bool skip = false;
	for (int p = 3; ; p += 2) {
		skip = false;
		for (int i = 3; i*i <= p; i+=2) {
			if (p % i == 0) {
				skip = true;
				break;
			}
		}
		if (!skip) {
			++prime_counter;
			if (prime_counter == num)
				return p;
		}
	}
}

int main() {
	int pipefd1[2];
	pipe(pipefd1);
	int pipefd2[2];
	pipe(pipefd2);
	int cpid = fork();
	if (cpid) {
		std::string input;
		while (true) {
			std::cout << "[Parent] Please enter the number: ";
			std::cin >> input;
			if (input == "exit") {
				kill(cpid, 9);
				close(pipefd1[0]);
				close(pipefd1[1]);
				close(pipefd2[0]);
				close(pipefd2[1]);
				exit(0);
			}
			std::cout << "[Parent] Sending " << input << " to the child process...\n";
			std::cout << "[Parent] Waiting for the response from the child process...\n";
			volatile int input_int = atoi(input.data());
			write(pipefd1[1], (char*)&input_int, 4);
			volatile int output;
			read(pipefd2[0], (char*)&output, 4);
			std::cout << "[Parent] Received calculation result of prime(" << input_int << ") = " << output << "...\n";
		}
	}
	else {
		while (true) {
			volatile int input;
			read(pipefd1[0], (char*)&input, 4);
			std::cout << "[Child] Calculating " << input << "-th prime number...\n";
			int result = prime(input);
			std::cout << "[Child] Sending calculation result of prime(" << input << ")...\n";
			write(pipefd2[1], (char*)&result, 4);
		}
	}
}
