//
// Created by tomas on 07/09/17.
//
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include "MemoriaCompartida.h"
#include "MemoriaCompartida2.h"

#include "SHMUsage.h"

int SHMUsage::calcularRandom() {
    srand ( (unsigned int) time(NULL) );
    int resultado = rand() % 100 + 1;
    return resultado;
}

void SHMUsage::useOne() {
    pid_t procId = fork ();

    if ( procId == 0 ) {

        std::string archivo ( "/bin/bash" );
        MemoriaCompartida<int> memoria;
        int estadoMemoria = memoria.crear ( archivo,'R' );

        std::cout << "Hijo: duermo 5 segundos..." << std::endl;
        sleep ( 5 );

        if ( estadoMemoria == SHM_OK ) {
            int resultado = memoria.leer ();
            std::cout << "Hijo: leo el numero " << resultado << " de la memoria compartida" << std::endl;
            memoria.liberar ();
        } else {
            std::cout << "Hijo: error en memoria compartida: " << estadoMemoria << std::endl;
        }
        std::cout << "Hijo: fin del proceso" << std::endl;
        exit ( 0 );

    } else {

        std::string archivo ( "/bin/bash" );
        MemoriaCompartida<int> memoria;
        int estadoMemoria = memoria.crear ( archivo,'R' );
        if ( estadoMemoria == SHM_OK ) {

            // escribe un dato para el hijo
            int random = calcularRandom ();
            std::cout << "Padre: escribo el numero " << random << " en la memoria compartida" << std::endl;
            memoria.escribir ( random );

            // espera a que termine el hijo
            wait ( NULL );
            // libera la memoria
            memoria.liberar ();
        } else {
            std::cout << "Padre: error en memoria compartida: " << estadoMemoria << std::endl;
        }

        std::cout << "Padre: fin del proceso" << std::endl;
        exit ( 0 );

    }
}

void SHMUsage::useTwo() {
    std::string archivo = "/bin/ls";
    pid_t procId = fork ();

    if ( procId == 0 ) {
        // codigo del hijo
        try {
            MemoriaCompartida2<int> buffer ( archivo,'A' );

            std::cout << "Hijo: duermo 5 segundos..." << std::endl;
            sleep ( 5 );

            int resultado = buffer.leer ();
            std::cout << "Hijo: leo el numero " << resultado << " de la memoria compartida" << std::endl;
            std::cout << "Hijo: fin del proceso" << std::endl;
        } catch ( std::string& mensaje ) {
            std::cerr << mensaje << std::endl;
        }

        exit(0);
    } else {
        // codigo del padre
        try {
            MemoriaCompartida2<int> buffer ( archivo,'A' );

            int random = calcularRandom ();
            std::cout << "Padre: escribo el numero " << random << " en la memoria compartida" << std::endl;
            buffer.escribir(random);

            // espera a que termine el hijo
            std::cout << "Padre: esperando a que termine el hijo" << std::endl;
            wait(NULL);

        } catch ( std::string& mensaje ) {
            std::cerr << mensaje << std::endl;
        }

        exit(0);
    }
}