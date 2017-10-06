#include "WorkerProcess.h"
#include "../ipc/SignalHandler.h"
#include "../utils/Logger.h"

WorkerProcess::~WorkerProcess() {
}

void WorkerProcess::initialize() {
}

void WorkerProcess::finalize() {
}

int WorkerProcess::loop() {

    //SIGIntHandler sigint_handler;


    // se registra el event handler declarado antes
    SignalHandler :: getInstance()->registrarHandler ( SIGINT,&sigint_handler );
    Logger::log(prettyName(), Logger::DEBUG, "SIGIntHandler registrado", Logger::get_date());
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

sig_atomic_t WorkerProcess::graceQuit() const {
    return this->sigint_handler.getGracefulQuit();
}
