#ifdef EJEMPLO_1

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "Semaforo.h"
#include "MemoriaCompartida.h"

using namespace std;

int calcularRandom ();

int main () {

	static const string NOMBRE = "main1.cc";
	static const string SHM = "Semaforo.h";
	static const char LETRA = 'a';

	MemoriaCompartida<int> buffer;
	Semaforo semaforo ( NOMBRE,0 );

	int pid = fork ();

	if ( pid == 0 ) {

		// lector
		buffer.crear ( SHM,LETRA );
		cout << "Lector: esperando por el semaforo . . ." << endl;
		semaforo.p ();
		int resultado = buffer.leer ();
		buffer.liberar ();
		cout << "Lector: tome el semaforo - valor leido = " << resultado << endl;
		exit ( 0 );

	} else {

		// escritor
		buffer.crear ( SHM,LETRA );
		int aDormir = calcularRandom ();
		cout << "Escritor: durmiendo " << aDormir << " segundos hasta liberar el semaforo" << endl;
		sleep ( aDormir );
		buffer.escribir ( aDormir );
		semaforo.v ();
		cout << "Escritor: libere el semaforo" << endl;

		// espero a que finalice el hijo antes de liberar los recursos
		wait ( NULL );

		buffer.liberar ();
		semaforo.eliminar ();
		exit ( 0 );
	}
}

int calcularRandom () {
	srand ( time(NULL) );
	int resultado = rand() % 10 + 1;
	return resultado;
}

#endif
