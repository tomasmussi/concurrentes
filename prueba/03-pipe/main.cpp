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


int main(int argc, char* argv[]) {
	int pipefd[2];
	int rta = pipe(pipefd);
	if (rta == -1) {
		std::cout << "fallo el pipe: " << std::strerror(errno) << std::endl;
		_exit(1);
	}
	pid_t pid = fork();
	if (pid == 0) {
		// Hijo
		close(pipefd[0]); // Cierro lectura
		char buffer[100];

		for (int i = 0; i < 10; i++) {
			sprintf(buffer, "%02d", i);
			std::cout << "escrito: " << buffer << std::endl;
			write(pipefd[1], &buffer, 2);
			sleep(2);
		}
		close(pipefd[1]);
		_exit(0);
	} else {
		// Padre
		// int pipefd[2];

		close(pipefd[1]); // Cierro escritura
		char buffer[100];
		for (int i = 0; i < 10; i++) {
			read(pipefd[0], &buffer, 2);
			std::cout << "lei: " << buffer << std::endl;
		}
		close(pipefd[0]);
		wait(NULL);
		_exit(0);
	}
	return 0;
}