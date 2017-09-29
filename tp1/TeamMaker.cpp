//
// Created by tomas on 29/09/17.
//

#include "TeamMaker.h"
#include "Person.h"

#include <iostream>
#include <unistd.h>
#include <cstring>
#include <errno.h>

TeamMaker::TeamMaker(const std::string& fifo_read, const std::string& fifo_write)
        : _fifo_read(fifo_read), _fifo_write(fifo_write), _fifo_prueba(fifo_read) {
    std::cout << "construyendo TeamMaker" << std::endl;

}

int TeamMaker::do_work() {
    std::cout << "pre work" << std::endl;
    Person p1(-1);
    ssize_t read = _fifo_read.leer(static_cast<void*>(&p1), sizeof(Person));
    if (read) {
        std::cout << "persona: " << p1.id() << std::endl;
    }
    std::cout << p1.id() << std::endl;
    return 0;
}

void TeamMaker::initialize() {
    std::cout << "abriendo fifos" << std::endl;
    _fifo_read.abrir();
    //_fifo_write.abrir();
    std::cout << "fin abrir fifos" << std::endl;
}

void TeamMaker::finalize() {
    _fifo_read.cerrar();
    _fifo_write.cerrar();
    _fifo_write.eliminar();


    _fifo_prueba.cerrar();
}


