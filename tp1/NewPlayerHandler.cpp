#include <signal.h>
#include <iostream>
#include <sstream>
#include "NewPlayerHandler.h"
#include "Logger.h"
#include "Person.h"

NewPlayerHandler::NewPlayerHandler(const std::string& fifo_write)
        : _pipe_writer(fifo_write), _i(0) {
}

NewPlayerHandler::~NewPlayerHandler() {
}

int NewPlayerHandler::handleSignal ( int signum ) {
    // TODO: Aca o en el constructor?
    _pipe_writer.abrir();
    Logger::log("NewPlayerHandler", Logger::DBG, "Handleando nuevo player");
    _pipe_writer.escribir(static_cast<void*>(&_i), sizeof(int));
    std::stringstream ss;
    ss << "Escribi en el fifo1: " << _i;
    std::string s = ss.str();
    Logger::log("NewPlayerHandler", Logger::DBG, s);
    _i++;
    return 0;
}
