//
// Created by tomas on 26/09/17.
//

#include "PeopleRegisterWorker.h"
#include <iostream>

PeopleRegisterWorker::PeopleRegisterWorker() : _shared_memory("/bin/bash", 'a'), i(0),
    _pipe_reader("/tmp/archivo_fifo") {
}

PeopleRegisterWorker::~PeopleRegisterWorker() {
}

int PeopleRegisterWorker::do_work() {
    int j;
    _pipe_reader.leer(&j, sizeof(int));
    std::cout << "fifo: " << j << std::endl;
    sleep(3);
    return 5;
}

void PeopleRegisterWorker::initialize() {
    _pipe_reader.abrir();
}
