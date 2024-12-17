#include <iostream>
#include <stdio.h>
#include <signal.h>
#include <pwd.h>
#include <unistd.h>

void handler(int signum, siginfo_t* info, void* context) {
	//Received a SIGUSR1 signal from process [PID] executed by [UID] ([username]).
	//State of the context: EIP = [value of EIP], EAX = [value of EAX], EBX = [value of EBX]. 
	std::cout << "Received a SIGUSR1 signal from process " << info->si_pid << " executed by " << info->si_uid << " (" << getpwuid(info->si_uid)->pw_name << ").\n";
	std::cout << "State of the context: EIP = " << static_cast<size_t>(reinterpret_cast<ucontext_t*>(context)->uc_mcontext.gregs[REG_RIP]) << ", EAX = " << static_cast<size_t>(reinterpret_cast<ucontext_t*>(context)->uc_mcontext.gregs[REG_RAX]) << ", EBX = " << static_cast<size_t>(reinterpret_cast<ucontext_t*>(context)->uc_mcontext.gregs[REG_RBX]) << ".\n";
}

int main() {
	struct sigaction act;
	act.sa_sigaction = &handler;
	act.sa_flags = SA_SIGINFO;
	if (sigaction(SIGUSR1, &act, nullptr) == -1) {
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
	while (true) {
		sleep(5);
	}
}
