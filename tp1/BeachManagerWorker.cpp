//
// Created by tomas on 26/09/17.
//

#include "BeachManagerWorker.h"
#include <iostream>
#include <unistd.h>

BeachManagerWorker::BeachManagerWorker() : _shared_memory("/bin/bash", 'a'),
    _pipe_writer("/tmp/archivo_fifo") {
}

BeachManagerWorker::~BeachManagerWorker() {
}

int BeachManagerWorker::do_work() {
    sleep(2);
    int i = 8;
    _pipe_writer.escribir(&i, sizeof(int));
    std::cout << "escribi fifo: " << i << std::endl;
    return 3;
}

void BeachManagerWorker::initialize() {
    _pipe_writer.abrir();
}

void BeachManagerWorker::finalize() {
    _pipe_writer.cerrar();
    _pipe_writer.eliminar();
}
