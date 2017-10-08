#include <signal.h>
#include "MainSIGIntHandler.h"
#include "../utils/Logger.h"

MainSIGIntHandler::MainSIGIntHandler() : _pid_list() {
}

MainSIGIntHandler::~MainSIGIntHandler() {
}

int MainSIGIntHandler::handleSignal(int signum) {
    Logger::log("main", Logger::INFO, "Recibido SIGINT, enviando señal al resto de los procesos", Logger::get_date());
    for (std::list<pid_t>::iterator it = _pid_list.begin(); it != _pid_list.end(); ++it) {
        kill(*it, SIGINT);
    }
    return 0;
}

void MainSIGIntHandler::add_pid_notification(pid_t pid) {
    _pid_list.push_back(pid);
}
