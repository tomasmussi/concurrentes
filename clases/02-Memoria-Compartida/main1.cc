#ifdef EJEMPLO_1

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include "MemoriaCompartida.h"

using namespace std;

int calcularRandom ();

int main () {

	pid_t procId = fork ();

	if ( procId == 0 ) {

		string archivo ( "/bin/bash" );
		MemoriaCompartida<int> memoria;
		int estadoMemoria = memoria.crear ( archivo,'R' );

		cout << "Hijo: duermo 5 segundos..." << endl;
		sleep ( 5 );

		if ( estadoMemoria == SHM_OK ) {
			int resultado = memoria.leer ();
			cout << "Hijo: leo el numero " << resultado << " de la memoria compartida" << endl;
			memoria.liberar ();
		} else {
			cout << "Hijo: error en memoria compartida: " << estadoMemoria << endl;
		}
		cout << "Hijo: fin del proceso" << endl;
		exit ( 0 );

	} else {

		string archivo ( "/bin/bash" );
		MemoriaCompartida<int> memoria;
		int estadoMemoria = memoria.crear ( archivo,'R' );
		if ( estadoMemoria == SHM_OK ) {

			// escribe un dato para el hijo
			int random = calcularRandom ();
			cout << "Padre: escribo el numero " << random << " en la memoria compartida" << endl;
			memoria.escribir ( random );

			// espera a que termine el hijo
			wait ( NULL );
			// libera la memoria
			memoria.liberar ();
		} else {
			cout << "Padre: error en memoria compartida: " << estadoMemoria << endl;
		}

		cout << "Padre: fin del proceso" << endl;
		exit ( 0 );

	}

}

int calcularRandom () {
	srand ( (unsigned int) time(NULL) );
	int resultado = rand() % 100 + 1;
	return resultado;
}

#endif
