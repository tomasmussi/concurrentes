#ifdef EJEMPLO_3

#include <iostream>
#include <stdio.h>
#include "LockFile.h"

using namespace std;

int main () {
	static const char HIJO = 'H';
	static const char PADRE = 'P';
	static const int ITERACIONES = 2048;
	static const std::string NOMBRE_ARCHIVO = "salida.txt";

	pid_t pid = fork ();

	if (pid == 0) {
		LockFile lock(NOMBRE_ARCHIVO);
		lock.tomarLock ();

		for (int contador = 0; contador < ITERACIONES; contador++) {
			int resultado = lock.escribir(static_cast<const void *>(&HIJO), sizeof(HIJO));

			if (resultado != sizeof(HIJO)) {
				perror("Hijo: error al escribir en el archivo");
			}
		}

		lock.liberarLock();
		cout << "Hijo: fin del programa" << endl;
		return 0;

	} else {
		LockFile lock(NOMBRE_ARCHIVO);
		lock.tomarLock ();

		for (int contador = 0; contador < ITERACIONES; contador++) {
			int resultado = lock.escribir(static_cast<const void *>(&PADRE), sizeof(PADRE));

			if (resultado != sizeof(PADRE)) {
				perror("Padre: error al escribir en el archivo");
			}
		}

		lock.liberarLock();
		cout << "Padre: fin del programa" << endl;
		return 0;
	}
}

#endif
