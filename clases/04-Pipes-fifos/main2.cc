#ifdef EJEMPLO_2

#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <sys/wait.h>

#include "Pipes/Pipe.h"


int main () {

	static const int BUFFSIZE = 100;

	Pipe canal;
	int pid = fork ();

	if ( pid == 0 ) {

		// lector
		char buffer[BUFFSIZE];
		canal.setearModo ( Pipe::LECTURA );

		std::cout << "Lector: esperando para leer . . ." << std::endl;

		// se redirige la entrada estandar
		dup2 ( canal.getFdLectura(),0 );
		// se lee el dato del pipe
		std::cin.get ( buffer,BUFFSIZE );

		std::cout << "Lector: lei el dato [" << buffer << "] del pipe" << std::endl;
		std::cout << "Lector: fin del proceso" << std::endl;

		canal.cerrar ();
		exit ( 0 );

	} else {

		// escritor
		std::string dato = "Hola mundo pipes (segundo ejemplo)!!";
		canal.setearModo ( Pipe::ESCRITURA );

		sleep ( 5 );

		std::cout << "Escritor: escribo el dato [" << dato << "] en el pipe" <<std:: endl;

		// se redirige la salida estandar
		dup2 ( canal.getFdEscritura(),1 );
		// se escribe el dato en el pipe
		std::cout << dato << std::endl;

		// esperar a que el hijo termine
		wait ( NULL );

		canal.cerrar ();
		exit ( 0 );
	}
}

#endif
