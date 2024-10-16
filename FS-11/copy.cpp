#include <iostream>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main () {
	std::string src;
	std::string dst;
	int src_off_t;
	int dst_off_t;
	int bytes_to_copy;

	std::cout << "Enter the name of the source file:\n";
	std::cin >> src;
	std::cout << "Enter the name of the destination file:\n";
	std::cin >> dst;
	std::cout << "Enter the initial offset in the source file (in bytes):\n";
	std::cin >> src_off_t;
	std::cout << "Enter the initial offset in the destination file (in bytes):\n";
	std::cin >> dst_off_t;
	std::cout << "Enter the number of bytes to copy:\n";
	std::cin >> bytes_to_copy;

	char* buffer = new char[bytes_to_copy];

	int src_fd = open(src.data(), O_RDONLY);
	if (src_fd == -1) {
		delete[] buffer;
		std::cerr << "open error";
		exit(EXIT_FAILURE);
	}

	int dst_fd = open(dst.data(), O_WRONLY);
	if (dst_fd == -1) {
		delete[] buffer;
		std::cerr << "open error";
		exit(EXIT_FAILURE);
	}

	if (lseek(src_fd, src_off_t, SEEK_SET) == -1) {
		delete[] buffer;
		std::cerr << "lseek error";
		exit(EXIT_FAILURE);
	}

	if (lseek(dst_fd, dst_off_t, SEEK_SET) == -1) {
		delete[] buffer;
		std::cerr << "lseek error";
		exit(EXIT_FAILURE);
	}

	int bytes_read = read(src_fd, buffer, bytes_to_copy);
	if (bytes_read < bytes_to_copy) {
		delete[] buffer;
		std::cerr << "read error";
		exit(EXIT_FAILURE);
	}

	if (write(dst_fd, buffer, bytes_read) < bytes_read) {
		delete[] buffer;
		std::cout << "write error";
		exit(EXIT_FAILURE);
	}

	delete[] buffer;
	std::cout << "Successfully copied " << bytes_read << " bytes from the source file to the destination file.\n";
}
