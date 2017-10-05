#include <signal.h>
#include <iostream>
#include <sstream>
#include "NewPlayerHandler.h"
#include "../utils/Logger.h"
#include "../model/Person.h"

NewPlayerHandler::NewPlayerHandler(const std::string& fifo_write)
        : _pipe_writer(fifo_write), _i(0) {
}

NewPlayerHandler::~NewPlayerHandler() {
    _pipe_writer.cerrar();
    _pipe_writer.eliminar();
}

int NewPlayerHandler::handleSignal ( int signum ) {
    Logger::log(prettyName(), Logger::DBG, "Handleando nuevo player", Logger::get_date());
    _pipe_writer.escribir(static_cast<void*>(&_i), sizeof(int));
    std::stringstream ss;
    ss << "Escribi en el fifo1: " << _i;
    std::string s = ss.str();
    Logger::log(prettyName(), Logger::DBG, s, Logger::get_date());
    _i++;
    return 0;
}

void NewPlayerHandler::initialize() {
    Logger::log(prettyName(), Logger::DBG, "Inicializando pipe", Logger::get_date());
    _pipe_writer.abrir();
}

std::string NewPlayerHandler::prettyName() {
    return "New Player Handler";
}
