#ifdef EJEMPLO_3

#include <unistd.h>
#include <stdlib.h>
#include <iostream>

using namespace std;


int main () {

	cout << "Proceso padre: " << getpid() << endl;
	pid_t id1 = fork ();

	if ( id1 == 0 ) {
		cout << "Hijo: Hola, soy primer el proceso hijo.  Mi process ID es " << getpid() << " (" << getppid() << ")" << endl;
	}

	pid_t id2 = fork ();

	if ( id2 == 0 ) {
		cout << "Hijo: Hola, soy segundo el proceso hijo.  Mi process ID es " << getpid() << " (" << getppid() << ")" << endl;
	}

}

#endif
