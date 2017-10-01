//
// Created by tomas on 26/09/17.
//

#include "BeachManagerWorker.h"
#include "Person.h"
#include <iostream>
#include <unistd.h>

BeachManagerWorker::BeachManagerWorker(const std::string& fifo_write)
   : _shared_memory("/bin/bash", 'a'), _pipe_writer(fifo_write), _i(0) {
}

BeachManagerWorker::~BeachManagerWorker() {
}

int BeachManagerWorker::do_work() {
    sleep(2);
    Person p(_i++);
    _pipe_writer.escribir(static_cast<void*>(&p), sizeof(Person));
    return 0;
}

void BeachManagerWorker::initialize() {
    _pipe_writer.abrir();
}

void BeachManagerWorker::finalize() {
    _pipe_writer.cerrar();
    _pipe_writer.eliminar();
}

std::string BeachManagerWorker::prettyName() {
    return "Beach Manager Worker";
}
