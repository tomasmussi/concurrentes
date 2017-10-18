#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string>
#include <sys/wait.h>
#include <iostream>
#include <iostream>
#include <cmath>
#include <cerrno>
#include <cstring>
#include <clocale>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUF_SIZE 2

int main(int argc, char* argv[]) {
	pid_t pid = fork();
	if (pid == 0) {
		mknod("archivo", S_IFIFO | 0644, 0);
		int fd = open("archivo", O_WRONLY);

		char buffer[100];

		for (int i = 0; i < 10; i++) {
			sprintf(buffer, "%02d", i);
			std::cout << "escrito: " << buffer << std::endl;
			write(fd, &buffer, BUF_SIZE);
			// sleep(2);
		}
		close(fd);
		unlink("archivo");
		_exit(0);
	} else {
		// Padre
		mknod("archivo", S_IFIFO | 0644, 0);
		int fd = open("archivo", O_RDONLY);

		char buffer[100];
		for (int i = 0; i < 10; i++) {
			read(fd, &buffer, BUF_SIZE);
			std::cout << "lei: " << buffer << std::endl;
		}
		close(fd);

		wait(NULL);
		_exit(0);
	}
	return 0;
}