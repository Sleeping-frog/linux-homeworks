#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>

bool redirect = false;
int exit_code = 0;
int operator_code = 0; // 0 - ";", 1 - "&&", 2 - "||"
int redir_output_code = 0; // 0 - no redirection output, 1 - ">", 2 - ">>"
std::string redir_output_path;


void new_process(char** args) {
	if (std::string(args[0]) == "exit") {
		exit(0);
	}
	int pid = fork();
	if (pid == 0) {
		if (redir_output_code == 1) {
			if(close(1) == -1) {
				perror("close error");
				exit(-1);
			}
			if (creat(redir_output_path.data(), 0644) == -1) {
				perror("redirection error");
				exit(-1);
			}
		}
		if (redir_output_code == 2) {
			if (close(1) == -1) {
				perror("close error");
				exit(-1);
			}
			if (open(redir_output_path.data(), O_CREAT | O_WRONLY | O_APPEND , 0644) == -1) {
				perror("redirection error");
				exit(-1);
			}
		}
		if (redirect) {
			if (close(1) == -1) {
				perror("close error");
				exit(-1);
			}
			if (creat("./tmp.log", 0644) == -1) {
				perror("redirection error");
				exit(-1);
			}
		}
		if (execvp(args[0], args) == -1) {
			perror("execve error");
			exit(-1);
		}
	}
	else {
		wait(&exit_code);
		if (redirect) {
			std::string new_name = "./" + std::to_string(pid) + ".log";
			if (rename("./tmp.log", new_name.data()) == -1) {
				perror("renaming error");
				exit(-1);
			}
			redirect = false;
		}
	}
}

char** make_args(std::vector<std::string>& vec, int start, int end) {
	char** args = new char*[end - start + 1];
        args[end - start] = nullptr;
        for (int i = 0; i < end - start; ++i) {
                args[i] = vec[start + i].data();
        }

	if (std::string(args[0]) == "exit") {
                delete[] args;
                exit(0);
        }
	return args;
}



int main() {

	//adding current directory to PATH
	std::string path = getenv("PATH");
	char buffer[200];
	if (getcwd(buffer, 200) == 0) {
		perror("getcwd error");
		exit(-1);
	}
	path.insert(0, std::string(buffer) + ":");
	setenv("PATH", path.data(), 1);


	while (true) {
		//set color to cyan and then return to default
		std::cout << "\033[1m\033[36m" << "shell: " << "\033[0m";
		std::string raw_input;
		std::getline(std::cin, raw_input);
	
		std::vector<std::string> processed_input{""};
	
		int space_counter = 0;
		for (auto chr : raw_input) {
			if (chr == ' ') {
				processed_input.push_back("");
				++space_counter;
				continue;
			}
			processed_input[space_counter].push_back(chr);
		}
		

		char** args;
		int cur_pos = 0;
		int prev_operator_code = operator_code;
		bool execute = false;
		int redir_output_offset = 0;
		for (int i = 0; i < processed_input.size(); ++i) {
			if (processed_input[cur_pos] == "silent") {
                                redirect = true;
                                ++cur_pos;
                        }
			if (processed_input[i] == ";") {
				operator_code = 0;
				execute = true;
			}
			else if (processed_input[i] == "&&") {
				operator_code = 1;
				execute = true;
			}
			else if (processed_input[i] == "||") {
				operator_code = 2;
				execute = true;
			}
			else if (processed_input[i] == ">") {
				redir_output_offset = 2;
				redir_output_code = 1;
				redir_output_path = processed_input[++i];
			}
			else if (processed_input[i] == ">>") {
				redir_output_offset = 2;
				redir_output_code = 2;
				redir_output_path = processed_input[++i];
			}
			if (execute) {
				switch (prev_operator_code) {
	                                case 0:
	                                        args = make_args(processed_input, cur_pos, i - redir_output_offset);
	                                        new_process(args);
	                                        delete[] args;
	                                        break;
	                                case 1:
	                                        if (exit_code == 0) {
	                                                args = make_args(processed_input, cur_pos, i - redir_output_offset);
	                                                new_process(args);
	                                                delete[] args;
	                                        }
						break;
					case 2:
						if (exit_code != 0) {
							args = make_args(processed_input, cur_pos, i - redir_output_offset);
		                                        new_process(args);
		                                        delete[] args;
						}
						break;
	                        }
				cur_pos = i + 1;
				prev_operator_code = operator_code;
				execute = false;
				redir_output_code = 0;
				redir_output_offset = 0;
			}
		}
		switch (prev_operator_code) {
                        case 0:
                                args = make_args(processed_input, cur_pos, processed_input.size() - redir_output_offset);
                                new_process(args);
                                delete[] args;
                                break;
                        case 1:
                                if (exit_code == 0) {
                                        args = make_args(processed_input, cur_pos, processed_input.size() - redir_output_offset);
                                        new_process(args);
                                        delete[] args;
                                }
                                break;
                        case 2:
                                if (exit_code != 0) {
                                        args = make_args(processed_input, cur_pos, processed_input.size() - redir_output_offset);
                                        new_process(args);
                                        delete[] args;
                                }
                                break;
                }
		exit_code = 0;
		operator_code = 0;
		redir_output_code = 0;
		redir_output_offset = 0;
	}
}
