//
// Created by tomas on 26/09/17.
//

#include "WorkerProcess.h"
#include "SIGIntHandler.h"
#include "SignalHandler.h"

WorkerProcess::~WorkerProcess() {
}

void WorkerProcess::initialize() {
}

int WorkerProcess::loop() {
    SIGIntHandler sigint_handler;

    // se registra el event handler declarado antes
    SignalHandler :: getInstance()->registrarHandler ( SIGINT,&sigint_handler );
    this->initialize();
    // mientras no se reciba la senial SIGINT, el proceso realiza su trabajo
    int exit_status = 0;
    while ( sigint_handler.getGracefulQuit() == 0 ) {
        exit_status = do_work();
    }
    // se recibio la senial SIGINT, el proceso termina
    SignalHandler::destroy();
    return exit_status;
}


