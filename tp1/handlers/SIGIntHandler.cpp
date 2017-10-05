#include "SIGIntHandler.h"

SIGIntHandler::SIGIntHandler() : gracefulQuit(0) {
}

SIGIntHandler::~SIGIntHandler() {
}

int SIGIntHandler::handleSignal(int signum) {
    // assert ( signum == SIGINT );
    // std::cout << "["<<getpid()<<"] " << "handle signal en hijo" << std::endl;
    this->gracefulQuit = 1;
    return 0;
}

sig_atomic_t SIGIntHandler::getGracefulQuit() const {
    return this->gracefulQuit;
}