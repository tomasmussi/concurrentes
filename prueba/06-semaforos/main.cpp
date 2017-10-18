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


#define N 5

int main(int argc, char* argv[]) {

	key_t key = ftok("main.cpp", 'a');
	int semId = semget(key, 1, IPC_CREAT | 0644);

	union semun {
       int              val;    // Value for SETVAL
       struct semid_ds *buf;    // Buffer for IPC_STAT, IPC_SET
       unsigned short  *array;  // Array for GETALL, SETALL
       struct seminfo  *__buf;  // Buffer for IPC_INFO
	};
	semun asd;
	asd.val = 2; // Arranca en 1 el semaforo

	semctl(semId, 0, SETVAL, asd);

	for (int i = 0; i < N; i++) {
		pid_t pid = fork();

		if (pid == 0) {
			struct sembuf buf;
			memset(&buf, 0, sizeof(struct sembuf));
			buf.sem_num = 0;
			buf.sem_op = -1;
			//buf.sem_flg = SEM_UNDO;

			semop(semId, &buf, 1); // 1 solo semaforo
			std::cout << "[" << getpid() << "] semaforo tomado" << std::endl;
			sleep(4);

			buf.sem_op = 1;
			std::cout << "[" << getpid() << "] por liberar semaforo" << std::endl;
			semop(semId, &buf, 1);

			_exit(0);
		}
	}

	for (int i = 0; i < N; i++) {
		wait(NULL);
	}

	semctl(semId, 0,IPC_RMID);
	return 0;
}