//
// Created by tomas on 26/09/17.
//

#include "WorkerProcess.h"
#include "SIGIntHandler.h"
#include "SignalHandler.h"
#include <iostream>

WorkerProcess::~WorkerProcess() {
}

int WorkerProcess::loop() {
    SIGIntHandler sigint_handler;

    // se registra el event handler declarado antes
    EventHandler *hd = SignalHandler :: getInstance()->registrarHandler ( SIGINT,&sigint_handler );
    if (hd != NULL) {
        std::cout << "habia registrado algo previamente: " << hd << std::endl;
    }

    // mientras no se reciba la senial SIGINT, el proceso realiza su trabajo
    int exit_status = 0;
    while ( sigint_handler.getGracefulQuit() == 0 ) {
        /*cout << "Soy el proceso " << getpid() << endl;
        sleep ( 2 );*/
        exit_status = do_work();
    }

    // se recibio la senial SIGINT, el proceso termina
    SignalHandler::destroy();
    return exit_status;
}
