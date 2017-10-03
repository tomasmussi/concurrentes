//
// Created by tomas on 26/09/17.
//

#include "BeachManagerWorker.h"
#include "Person.h"
#include "Logger.h"

BeachManagerWorker::BeachManagerWorker(const std::string& fifo_write)
   : _shared_memory("/bin/bash", 'a'), _pipe_writer(fifo_write), _i(0) {
}

BeachManagerWorker::~BeachManagerWorker() {
}

int BeachManagerWorker::do_work() {

    if (_i < 12) {
        Person p(_i++);
        std::string timestamp = Logger::get_date();
        _pipe_writer.escribir(static_cast<void*>(&p), sizeof(Person));
        Logger::log(prettyName(), Logger::INFO, "Enviada persona " + p.id() + " para jugar", timestamp);
    }
    return 0;
}

void BeachManagerWorker::initialize() {
    _pipe_writer.abrir();
    Logger::log(prettyName(), Logger::DBG, "Fifo de envio de nuevas personas abierto" ,Logger::get_date());
}

void BeachManagerWorker::finalize() {
    _pipe_writer.cerrar();
    _pipe_writer.eliminar();
}

std::string BeachManagerWorker::prettyName() {
    return "Beach Manager Worker";
}
