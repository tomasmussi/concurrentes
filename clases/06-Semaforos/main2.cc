#ifdef EJEMPLO_2

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "Semaforo.h"
#include "MemoriaCompartida.h"


using namespace std;

int calcularRandom ();

int main () {

	static const string NOMBRE1 = "main1.cc";
	static const string NOMBRE2 = "main2.cc";
	static const string NOMBRE3 = "main3.cc";
	static const string SHM = "Semaforo.cpp";
	static const char LETRA = 'a';

	MemoriaCompartida<int> buffer;
	Semaforo semaforo1 ( NOMBRE1,0 );
	Semaforo semaforo2 ( NOMBRE2,0 );
	Semaforo semaforo3 ( NOMBRE3,0 );

	pid_t pid = fork ();

	if ( pid == 0 ) {

		int pid2 = fork ();

		if ( pid2 == 0 ) {

			int pid3 = fork ();

			if ( pid3 == 0 ) {

				// lector 1
				buffer.crear ( SHM,LETRA );

				cout << "Lector (pid " << getpid () << "): esperando por el semaforo" << endl;
				semaforo1.p ();

				int resultado = buffer.leer ();
				cout << "Lector (pid " << getpid() << "): tome el semaforo - valor leido = " << resultado << endl;

				buffer.liberar ();
				exit ( 0 );
			} else {

				// lector 2
				buffer.crear ( SHM,LETRA );

				cout << "Lector (pid " << getpid () << "): esperando por el semaforo" << endl;
				semaforo2.p ();

				int resultado = buffer.leer ();
				cout << "Lector (pid " << getpid() << "): tome el semaforo - valor leido = " << resultado << endl;

				buffer.liberar ();
				wait(NULL);
				exit ( 0 );
			}

		} else {

			// lector 3
			buffer.crear ( SHM,LETRA );

			cout << "Lector (pid " << getpid () << "): esperando por el semaforo" << endl;
			semaforo3.p ();

			int resultado = buffer.leer ();
			cout << "Lector (pid " << getpid() << "): tome el semaforo - valor leido = " << resultado << endl;

			buffer.liberar ();
			wait(NULL);
			exit ( 0 );
		}

	} else {

		// escritor
		buffer.crear ( SHM,LETRA );

		int aDormir = calcularRandom ();

		cout << "Escritor (pid " << getpid () << "): duerme " << aDormir << " segundos hasta liberar el semaforo" << endl;
		sleep ( aDormir );

		buffer.escribir ( aDormir );
		semaforo1.v ();
		semaforo2.v ();
		semaforo3.v ();
		cout << "Escritor: libere los semaforos" << endl;

		// espero a que terminen los hijos antes de liberar los recursos
		wait ( NULL );

		buffer.liberar ();
		semaforo1.eliminar ();
		semaforo2.eliminar ();
		semaforo3.eliminar ();
		exit ( 0 );
	}
}

int calcularRandom () {
	srand ( time(NULL) );
	int resultado = rand() % 10 + 1;
	return resultado;
}

#endif
