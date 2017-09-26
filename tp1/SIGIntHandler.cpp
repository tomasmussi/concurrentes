//
// Created by tomas on 26/09/17.
//

#include "SIGIntHandler.h"
#include <iostream>
#include <unistd.h>

SIGIntHandler::SIGIntHandler() : gracefulQuit(0) {
}

SIGIntHandler::~SIGIntHandler () {
}

int SIGIntHandler::handleSignal ( int signum ) {
    // assert ( signum == SIGINT );
    std::cout << "["<<getpid()<<"]" << "handle signal en hijo probablemente" << std::endl;
    this->gracefulQuit = 1;
    return 0;
}

sig_atomic_t SIGIntHandler::getGracefulQuit () const {
    return this->gracefulQuit;
}