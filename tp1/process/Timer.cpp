#include "Timer.h"
#include "../constants.h"
#include "../ipc/SignalHandler.h"
#include "../utils/Logger.h"
#include <sstream>

Timer::Timer(Semaphore& tournament_started) : _already_finished(false), _tournament_started(tournament_started) {
}

Timer::~Timer() {
}

int Timer::handleSignal(int signum) {
    if (signum == SIGUSR2) {
        Logger::log(prettyName(), Logger::DEBUG, "Se despacho un partido, por lo que hubo actividad", Logger::get_date());
    }
}

int Timer::do_work() {
    // El timer empieza a correr desde que empieza el torneo
    _tournament_started.p();
    int remaining = MAX_WAIT_TIME;
    while (remaining != 0 && graceQuit() == 0 && !_already_finished) {
        remaining = sleep(MAX_WAIT_TIME);
        std::stringstream ss;
        ss << "Faltaban " << remaining << " segundos para que termine el torneo";
        Logger::log(prettyName(), Logger::INFO, ss.str(), Logger::get_date());
    }
    // Hubo MAX_WAIT_TIME segundos sin partidos empezados
    if (graceQuit() == 0 && !_already_finished) { // No recibio SIGINT
        Logger::log(prettyName(), Logger::INFO, "Terminando el torneo debido a falta de actividad", Logger::get_date());
        kill(getppid(), SIGINT);
        _already_finished = true;
    }
}

void Timer::initialize() {
    // Se registra simplemente para que interrumpa el sleep
    SignalHandler::getInstance()->registrarHandler(SIGUSR2, this);
    Logger::log(prettyName(), Logger::INFO, "Inicializado", Logger::get_date());
}

void Timer::finalize() {
    _tournament_started.remove();
    Logger::log(prettyName(), Logger::DEBUG, "Semaforo removido", Logger::get_date());
    SignalHandler::destroy();
    Logger::log(prettyName(), Logger::INFO, "Finalizado", Logger::get_date());
}

std::string Timer::prettyName() {
    return "Timer";
}

