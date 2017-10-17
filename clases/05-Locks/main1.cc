#ifdef EJEMPLO_1

#include <iostream>
#include <stdlib.h>

#include "LockFile.h"

using namespace std;

int main () {

	pid_t processId = fork ();

	if ( processId == 0 ) {
		// hijo
		cout << "Empieza el hijo" << endl;
		LockFile lock ( "main1.cc" );

		sleep ( 1 );
		cout << "Hijo: esperando para tomar lock..." << endl;
		lock.tomarLock();
		cout << "Hijo: tome el lock" << endl;
		sleep ( 10 );
		lock.liberarLock();
		cout << "Hijo: libere el lock y termino" << endl;

		return ( 0 );

	} else {
		// padre
		cout << "Empieza el padre" << endl;
		LockFile lock ( "main1.cc" );

		lock.tomarLock();
		cout << "Padre: tome el lock" << endl;
		sleep ( 5 );
		lock.liberarLock();
		cout << "Padre: libere el lock y termino" << endl;

		return ( 0 );
	}
}

#endif
