#ifdef EJEMPLO_3

#include <iostream>
#include <stdlib.h>
#include <sys/wait.h>

#include "Fifos/FifoLectura.h"
#include "Fifos/FifoEscritura.h"


int main () {

	static const int BUFFSIZE = 100;
	static const std::string ARCHIVO_FIFO = "/tmp/archivo_fifo";

	pid_t pid = fork ();

	if ( pid == 0 ) {
		FifoLectura canal ( ARCHIVO_FIFO );
		char buffer[BUFFSIZE];

		canal.abrir();
		std::cout << "[Lector] A punto de leer del fifo" << std::endl;
		ssize_t bytesLeidos = canal.leer(static_cast<void*>(buffer),BUFFSIZE);

		std::string mensaje = buffer;
		mensaje.resize ( bytesLeidos );
		std::cout << "[Lector] Lei el dato del fifo: " << mensaje << std::endl;

		canal.cerrar();
		std::cout << "[Lector] Fin del proceso" << std::endl;
		exit ( 0 );
	} else {
		FifoEscritura canal ( ARCHIVO_FIFO );
		std::string mensaje = "Hola mundo fifo!!";

		canal.abrir();
		sleep ( 5 );
		canal.escribir ( static_cast<const void*>(mensaje.c_str()),mensaje.length() );
		std::cout << "[Escritor] Escribi el mensaje " << mensaje << " en el fifo" << std::endl;

		// esperar a que el hijo termine
		wait ( NULL );

		canal.cerrar ();
		canal.eliminar ();

		std::cout << "[Escritor] Fin del proceso" << std::endl;
		exit ( 0 );
	}

}

#endif
