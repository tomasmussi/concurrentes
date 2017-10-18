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

#define MAX 100

int main(int argc, char* argv[]) {
	pid_t pid = fork();

	if (pid == 0) {
		int fd = open("archivo", O_CREAT|O_WRONLY, 0644);
		struct flock fl;
		memset(&fl, 0, sizeof(struct flock));

		fl.l_type = F_WRLCK;
		fl.l_whence = SEEK_SET;
		fl.l_start = 0;
		fl.l_len = 0;

		fcntl(fd, F_SETLKW, &fl);
		// lock tomado
		std::cout << "Hijo, lock tomado" << std::endl;
		for (int i = 0; i < MAX; i++) {
			std::cout << i << std::endl;
		}
		std::cout << "Hijo, lock liberado" << std::endl;
		fl.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &fl);
		close(fd);

		_exit(0);
	} else {
		// Padre
		int fd = open("archivo", O_CREAT|O_WRONLY, 0644);
		struct flock fl;
		memset(&fl, 0, sizeof(struct flock));

		fl.l_type = F_WRLCK;
		fl.l_whence = SEEK_SET;
		fl.l_start = 0;
		fl.l_len = 0;
		fcntl(fd, F_SETLKW, &fl);

		std::cout << "Padre, lock tomado" << std::endl;
		for (int i = 0; i < MAX; i++) {
			std::cout << i << std::endl;
		}
		std::cout << "Padre, lock liberado" << std::endl;
		fl.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &fl);

		wait(NULL);
		_exit(0);
	}
	return 0;
}