//
// Created by tomas on 26/09/17.
//

#include "WorkerProcess.h"
#include "SIGIntHandler.h"
#include "SignalHandler.h"
#include "Logger.h"

WorkerProcess::~WorkerProcess() {
}

void WorkerProcess::initialize() {
}

void WorkerProcess::finalize() {
}

int WorkerProcess::loop() {
    Logger::log(prettyName(), Logger::DBG, "Comienzo loop");
    SIGIntHandler sigint_handler;

    // se registra el event handler declarado antes
    SignalHandler :: getInstance()->registrarHandler ( SIGINT,&sigint_handler );
    Logger::log("main", Logger::DBG, "SIGIntHandler registrado");
    this->initialize();
    // mientras no se reciba la senial SIGINT, el proceso realiza su trabajo
    int exit_status = 0;
    while ( sigint_handler.getGracefulQuit() == 0 ) {
        exit_status = do_work();
    }
    this->finalize();
    // se recibio la senial SIGINT, el proceso termina
    SignalHandler::destroy();
    return exit_status;
}



