#ifdef EJEMPLO_2

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

using namespace std;

int calcularRandom ();


int main () {

	pid_t id = fork ();

	if ( id == 0 ) {

		int tiempoAdormir = calcularRandom ();
		cout << "Hijo: Hola, soy el proceso hijo.  Voy a dormir " << tiempoAdormir << " segundos" << endl;
		sleep ( tiempoAdormir );
		cout << "Hijo: ya me desperte y termino" << endl;

		exit ( 0 );

	} else {

		cout << "Padre: Hola, soy el proceso padre.  Espero a que mi hijo se despierte" << endl;
		int estado;
		wait ( &estado );
		cout << "Padre: mi hijo se desperto y termino" << endl;

		exit ( 0 );
	}
}


int calcularRandom () {
	srand ( time(NULL) );
	int resultado = rand() % 10 + 1;
	return resultado;
}

#endif
