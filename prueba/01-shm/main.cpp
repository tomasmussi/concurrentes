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
		// Hijo, escribe la memoria
		key_t key = ftok("/bin/bash", 'R');
		int shmId = shmget(key, sizeof(int), IPC_CREAT | IPC_EXCL | 0644);
		if (shmId == -1) {
			shmId = shmget(key, sizeof(int), IPC_CREAT | 0644);
			if (shmId == -1) {
				std::cout << "no se puede iniciar shm" << std::endl;
				_exit(1);
			}
		}
		int *pointer = static_cast<int*>(shmat(shmId, NULL, 0));
		*pointer = 24;
		sleep(5);
		*pointer = 8;
		shmdt(pointer);
		_exit(0);

	} else {
		// Lee de la memoria
		key_t key = ftok("/bin/bash", 'R');
		int shmId = shmget(key, sizeof(int), IPC_CREAT | IPC_EXCL | 0644);
		if (shmId == -1) {
			shmId = shmget(key, sizeof(int), IPC_CREAT | 0644);
			if (shmId == -1) {
				std::cout << "no se puede iniciar shm" << std::endl;
				_exit(1);
			}
		}
		int *pointer = static_cast<int*>(shmat(shmId, NULL, 0));

		int cont = 0;
		int anterior = 0;
		while (cont < 2) {
			// Busy wait....
			int nuevo = *pointer;
			if (nuevo != anterior) {
				std::cout << "lei de shm " << nuevo << std::endl;
				anterior = nuevo;
				cont++;
			}
		}
		int stat;
		wait(&stat);
		std::cout << "hijo salio con " << WEXITSTATUS(stat) << std::endl;
		shmdt(pointer);

		shmid_ds stru;
		shmctl(shmId, IPC_STAT, &stru);
		if (stru.shm_nattch != 0) {
			std::cout << "Todavia hay alguien attacheado a la shm" << std::endl;
			_exit(1);
		}
		shmctl(shmId, IPC_RMID, NULL);
		std::cout << "todo salio bien" << std::endl;
		_exit(0);
	}
	return 0;
}