#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>


void do_function(int code, double time) {
        printf("Completed with %d exit code and took %g seconds.\n", code, time);
}


void do_command(char** argv, char* name) {
	clock_t start, end;
	int pid = fork();
	start = clock();
        if (pid == -1) {
                perror("fork error");
                exit(-1);
        }
        if (pid == 0) {
                if (execvp(name, argv) == -1) {
                        perror("exevp error");
                        exit(-1);
                }
        }
        else {
		int code;
                wait(&code);
		end = clock();
		do_function(code, ((double)end - start)/CLOCKS_PER_SEC);
        }
}

	
int main (int argc, char** argv) {
	if (argc < 2) {
		printf("Not enough arguments\n");
		exit(-1);
	}
	char** new_argv = (char**)malloc(argc * sizeof(char**));
	new_argv[argc - 1] = nullptr;
	for (int i = 0; i < argc - 1; ++i) {
		new_argv[i] = argv[i + 1];
	}
	do_command(new_argv, argv[1]);
	free(new_argv);
}
