//
// Created by tomas on 26/09/17.
//

#include <signal.h>
#include "MainSIGIntHandler.h"

MainSIGIntHandler::MainSIGIntHandler() : _pid_list() {
}

MainSIGIntHandler::~MainSIGIntHandler() {
}

#include <iostream>
int MainSIGIntHandler::handleSignal ( int signum ) {
    std::cout << "["<<getpid()<<"]" << "handle signal en padre probablemente" << std::endl;
    for (std::list<pid_t>::iterator it = _pid_list.begin(); it != _pid_list.end(); ++it) {
        kill(*it, SIGINT);
    }
    return 0;
}

void MainSIGIntHandler::add_pid_notification(pid_t pid) {
    _pid_list.push_back(pid);
}

