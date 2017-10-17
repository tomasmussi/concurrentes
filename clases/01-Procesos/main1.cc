#ifdef EJEMPLO_1

#include <iostream>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

int main () {

	pid_t id = fork ();

	if ( id == 0 ) {

		cout << "Hijo: Hola, soy el proceso hijo.  Mi process ID es " << getpid() << endl;
		cout << "Hijo: El process ID de mi padre es " << getppid() << endl;

		exit ( 0 );

	} else {

		cout << "Padre: Hola, soy el proceso padre.  Mi process ID es " << getpid() << endl;
		cout << "Padre: El process ID de mi hijo es " << id << endl;

		exit ( 0 );
	}
}

#endif
