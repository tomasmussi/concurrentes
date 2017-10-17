#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string>
#include <sys/wait.h>
#include <iostream>


int main(int argc, char* argv[]) {
	pid_t pid = fork();
	if (pid == 0) {
		// Hijo

	} else {
		// Padre
	}
	return 0;
}