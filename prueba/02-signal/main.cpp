#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <iostream>
#include <string.h>

typedef void (*manejador)(int asd);

bool seguir = true;

void hacer(int sigint) {
	std::cout << "handling..." << std::endl;
	sleep(5);
	std::cout << "fin sleep" << std::endl;
	seguir = false;
}

int main(int argc, char* argv[]) {
	struct sigaction st;
	memset(&st, 0, sizeof(struct sigaction));

	st.sa_handler = hacer; // Handler
	sigemptyset(&st.sa_mask);
	sigaddset(&st.sa_mask, SIGINT); // Block sigint
	// sigaddset(&st.sa_mask, SIGTSTP);

	// SA_NODEFER permite que se interrumpa el handler
	// SA_RESETHAND una vez que se ejecuta 1 vez el handler, se defaultea la accion y no se llama
	// nunca mas

	// st.sa_flags = SA_NODEFER;

	sigaction(SIGINT, &st, NULL);


	while (seguir) {
		std::cout << "Durimiendo otros 4 segundos... " << std::endl;
		sleep(4);
	}
	std::cout << "Sali del while"<< std::endl;
	sleep(4);
	return 0;
}