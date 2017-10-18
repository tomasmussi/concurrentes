#include <unistd.h>
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
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>


#define MAX 100

int main(int argc, char* argv[]) {

	key_t key = ftok("main.cpp", 'a');
	int semId = semget(key, 1, O_CREAT | 0644);

	union semun {
       int              val;    /* Value for SETVAL */
       struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
       unsigned short  *array;  /* Array for GETALL, SETALL */
       struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                   (Linux-specific) */
	};
	semun asd;
	asd.val = 3;

	semctl(semId, 0, IPC_SET, asd);

	pid_t pid = fork();

	if (pid == 0) {


		_exit(0);
	} else {

		wait(NULL);
		_exit(0);
	}
	return 0;
}