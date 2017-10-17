#ifdef EJEMPLO_3

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "Semaforo.h"
#include "MemoriaCompartida.h"


using namespace std;

int calcularRandom ();

int main () {

	static const string NOMBRE = "main3.cc";
	static const string SHM = "Semaforo.cpp";
	static const char LETRA = 'a';
	static const int HIJOS = 3;

	MemoriaCompartida<int> buffer;
	Semaforo semaforo ( NOMBRE,0 );

	for ( int i=0;i<HIJOS;i++ ) {
		pid_t pid = fork();

		if ( pid == 0 ) {
			// lector n
			buffer.crear ( SHM,LETRA );

			cout << "Lector (pid " << getpid () << "): esperando por el semaforo" << endl;
			semaforo.p ();

			int resultado = buffer.leer ();
			buffer.liberar ();
			cout << "Lector (pid " << getpid() << "): tome el semaforo - valor leido = " << resultado << endl;
			exit ( 0 );
		}
	}

	// escritor
	buffer.crear ( SHM,LETRA );
	int aDormir = calcularRandom ();

	cout << "Escritor (pid " << getpid () << "): duerme " << aDormir << " segundos hasta liberar el semaforo" << endl;
	sleep ( aDormir );

	buffer.escribir ( aDormir );
	for ( int i=0;i<HIJOS;i++ ) {
		semaforo.v();
	}
	cout << "Escritor: libere el semaforo" << endl;

	// espero a que terminen los hijos antes de liberar los recursos
	for ( int i=0;i<HIJOS;i++ ) {
		wait(NULL);
	}

	buffer.liberar ();
	cout << "Escritor: Buffer liberado" << endl;
	semaforo.eliminar ();

	exit ( 0 );
}

int calcularRandom () {
	srand ( time(NULL) );
	int resultado = rand() % 10 + 1;
	return resultado;
}

#endif
