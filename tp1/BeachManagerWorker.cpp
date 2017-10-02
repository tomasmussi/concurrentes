//
// Created by tomas on 26/09/17.
//

#include "BeachManagerWorker.h"
#include "Person.h"
#include "Logger.h"
#include <iostream>
#include <unistd.h>
#include <sstream>

BeachManagerWorker::BeachManagerWorker(const std::string& fifo_read, const std::string& fifo_write)
   : _shared_memory("/bin/bash", 'a'), _pipe_reader(fifo_read), _pipe_writer(fifo_write), _i(0) {
}

BeachManagerWorker::~BeachManagerWorker() {
}

int BeachManagerWorker::do_work() {
    int j;
    Logger::log("BeachManagerWorker", Logger::DBG, "Empezando a leer del fifo1");
    _pipe_reader.leer(&j, sizeof(int));
    std::stringstream ss;
    ss << "Lei del fifo1: " << j;
    std::string s = ss.str();
    Logger::log("BeachManagerWorker", Logger::DBG, s);
    Person p(j);
    Logger::log("BeachManagerWorker", Logger::DBG, "\"Escribiendo en el fifo 2\"");
//    _pipe_writer.escribir(static_cast<void*>(&p), sizeof(Person));
//    Logger::log("BeachManagerWorker", Logger::DBG, "Termine de escribir en el fifo2");
    return 0;
}

void BeachManagerWorker::initialize() {
    Logger::log("BeachManagerWorker", Logger::DBG, "Inicializando pipes");
    _pipe_reader.abrir();
//    _pipe_writer.abrir();
//    Logger::log("main", Logger::DBG, "Fin inicializacion pipes");
}

void BeachManagerWorker::finalize() {
    _pipe_reader.cerrar();
//    _pipe_writer.cerrar();
//    _pipe_writer.eliminar();
}

std::string BeachManagerWorker::prettyName() {
    return "Beach Manager Worker";
}
