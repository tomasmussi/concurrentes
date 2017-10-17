#ifdef EJEMPLO_2

#include <iostream>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

int main () {
	static const char HIJO = 'H';
	static const char PADRE = 'P';
	static const int ITERACIONES = 2048;
	static const std::string NOMBRE_ARCHIVO = "salida.txt";

	pid_t pid = fork ();

	if (pid == 0) {
		int descriptor = open(NOMBRE_ARCHIVO.c_str(),O_CREAT | O_WRONLY,0777);

		if (descriptor > 0) {
			for (int contador = 0; contador < ITERACIONES; contador++) {
				lseek(descriptor, 0, SEEK_END);
				int resultado = write(descriptor, static_cast<const void *>(&HIJO), sizeof(HIJO));

				if (resultado != sizeof(HIJO)) {
					perror("Hijo: error al escribir en el archivo");
				}
			}
			close(descriptor);
		} else {
			perror("Hijo: error al abrir archivo");
		}

		cout << "Hijo: fin del programa" << endl;
		return 0;

	} else {
		int descriptor = open(NOMBRE_ARCHIVO.c_str(),O_CREAT | O_WRONLY,0777);

		if (descriptor > 0) {
			for (int contador = 0; contador < ITERACIONES; contador++) {
				lseek(descriptor, 0, SEEK_END);
				int resultado = write(descriptor, static_cast<const void *>(&PADRE), sizeof(PADRE));

				if (resultado != sizeof(PADRE)) {
					perror("Padre: error al escribir en el archivo");
				}
			}
			close(descriptor);
		} else {
			perror("Padre: error al abrir archivo");
		}

		cout << "Padre: fin del programa" << endl;
		return 0;
	}

}

#endif
