//
// Created by tomas on 26/09/17.
//

#include "BeachManagerWorker.h"
#include "Person.h"
#include "Logger.h"
#include <sstream>

BeachManagerWorker::BeachManagerWorker(const std::string& fifo_read, const std::string& fifo_write)
   : _shared_memory("/bin/bash", 'a'), _pipe_reader(fifo_read), _pipe_writer(fifo_write), _i(0) {
}

BeachManagerWorker::~BeachManagerWorker() {
}

int BeachManagerWorker::do_work() {
    int j;
    Logger::log(prettyName(), Logger::DBG, "Esperando para leer del fifo1", Logger::get_date());
    _pipe_reader.leer(&j, sizeof(int));
    std::stringstream ss;
    ss << "Lei del fifo1: " << j;
    std::string s = ss.str();
    Logger::log(prettyName(), Logger::DBG, s, Logger::get_date());
    if (j < 12) {
        Person p(_i++);
        std::string timestamp = Logger::get_date();
        _pipe_writer.escribir(static_cast<void*>(&p), sizeof(Person));
        Logger::log(prettyName(), Logger::DBG, "Enviada persona " + p.id() + " para jugar", timestamp);
    }
    return 0;
}

void BeachManagerWorker::initialize() {
    Logger::log(prettyName(), Logger::DBG, "Inicializando pipes", Logger::get_date());
    _pipe_reader.abrir();
    _pipe_writer.abrir();
    Logger::log(prettyName(), Logger::DBG, "Fin inicializacion pipes", Logger::get_date());
}

void BeachManagerWorker::finalize() {
    _pipe_reader.cerrar();
    _pipe_writer.cerrar();
    _pipe_writer.eliminar();
}

std::string BeachManagerWorker::prettyName() {
    return "Beach Manager Worker";
}
