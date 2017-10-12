#include "Timer.h"
#include "../constants.h"
#include "../ipc/SignalHandler.h"
#include "../utils/Logger.h"
#include <sstream>

Timer::Timer() : already_finished(false) {
}

Timer::~Timer() {
}

int Timer::handleSignal(int signum) {
    if (signum == SIGUSR2) {
        Logger::log(prettyName(), Logger::DEBUG, "Me interrumpieron mientras dormia!", Logger::get_date());
    }
}

int Timer::do_work() {
    int remaining = 10;
    while (remaining != 0 && graceQuit() == 0 && !already_finished) {
        remaining = sleep(10);
        std::stringstream ss;
        ss << "Faltaban " << remaining << " segundos para que termine el torneo";
        Logger::log(prettyName(), Logger::INFO, ss.str(), Logger::get_date());
    }
    // Hubo 10 segundos sin partidos empezados
    if (graceQuit() == 0 && !already_finished) { // No recibio SIGINT
        Logger::log(prettyName(), Logger::INFO, "Terminando el torneo debido a falta de actividad", Logger::get_date());
        kill(getppid(), SIGINT);
        already_finished = true;
    }
}

void Timer::initialize() {
    // Se registra simplemente para que interrumpa el sleep
    SignalHandler::getInstance()->registrarHandler(SIGUSR2, this);
    Logger::log(prettyName(), Logger::INFO, "Inicializado", Logger::get_date());
}

void Timer::finalize() {
    SignalHandler::destroy();
    Logger::log(prettyName(), Logger::INFO, "Finalizado", Logger::get_date());
}

std::string Timer::prettyName() {
    return "Timer";
}

