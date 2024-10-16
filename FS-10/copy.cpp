#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main (int argc, char** argv) {
	if (argc != 3) {
		exit(1);
	}

	int src_fd = open(argv[1], O_RDONLY);
	if (src_fd == -1) {
		perror("Error openning file");
		exit(EXIT_FAILURE);
	}

	int src_fd_read = open(argv[1], O_RDONLY);
        if (src_fd == -1) {
                perror("Error openning file");
                exit(EXIT_FAILURE);
        }

	int dst_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (dst_fd == -1) {
                perror("Error openning file");
                exit(EXIT_FAILURE);
        }

	int count_data = 0;
	int count_hole = 0;

	int cur_pos = 0;
	char buffer[4096];
	int bytes_passed = 0;

	while (true) {
		bytes_passed = cur_pos;
		cur_pos = lseek(src_fd, cur_pos, SEEK_HOLE);
		bytes_passed = cur_pos - bytes_passed;
		if (cur_pos == -1) {
			if (errno == ENXIO)
				break;
			perror("Error");
			exit(EXIT_FAILURE);
		}
		count_data += bytes_passed;

		while (bytes_passed > 0) {
			if (read(src_fd_read, buffer, (bytes_passed > 4096 ? 4096 : bytes_passed)) == -1) {
				perror("Error");
	      			exit(EXIT_FAILURE);
			}
			if (write(dst_fd, buffer, (bytes_passed > 4096 ? 4096 : bytes_passed)) == -1) {
				perror("Error");
	        	        exit(EXIT_FAILURE);
			}
			bytes_passed -= (bytes_passed > 4096 ? 4096 : bytes_passed);
		}

		bytes_passed = cur_pos;
		cur_pos = lseek(src_fd, cur_pos, SEEK_DATA);
		bytes_passed = cur_pos - bytes_passed;
		if (cur_pos == -1) {
                        if (errno == ENXIO)
                                break;
			perror("Error");
			exit(EXIT_FAILURE);
		}
		count_hole += bytes_passed;
		if (lseek(src_fd_read, bytes_passed, SEEK_CUR) == -1) {
			perror("Error");
                        exit(EXIT_FAILURE);
		}
		if (lseek(dst_fd, bytes_passed, SEEK_CUR) == -1) {
			perror("Error");
                        exit(EXIT_FAILURE);
		}
	}

	close(src_fd);
	close(dst_fd);

	printf("copied %d bytes (data: %d, hole: %d).\n", count_data + count_hole, count_data, count_hole);
}
