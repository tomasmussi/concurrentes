#include "ResultsReporter.h"
#include "../ipc/SignalHandler.h"
#include "../utils/Logger.h"

ResultsReporter::ResultsReporter(const std::string& fifo_read):
    _fifo_read(fifo_read){
}

ResultsReporter::~ResultsReporter() {
}

int ResultsReporter::do_work() {
    Match match;
    _fifo_read.leer(static_cast<void*>(&match), sizeof(Match));
    Logger::log(prettyName(), Logger::INFO, match.to_string(), Logger::get_date());
    return 0;
}

void ResultsReporter::initialize() {
    Logger::log(prettyName(), Logger::DEBUG, "Inicializando", Logger::get_date());
    _fifo_read.abrir();
    Logger::log(prettyName(), Logger::DEBUG, "Fifo de lectura de resultados de ResultsReporter abierto", Logger::get_date());
    Logger::log(prettyName(), Logger::INFO, "Inicializado", Logger::get_date());
}

void ResultsReporter::finalize() {
    Logger::log(prettyName(), Logger::DEBUG, "Finalizando", Logger::get_date());
    _fifo_read.cerrar();
    Logger::log(prettyName(), Logger::DEBUG, "Fifo cerrado", Logger::get_date());
    Logger::log(prettyName(), Logger::INFO, "Finalizado", Logger::get_date());
}

std::string ResultsReporter::prettyName() {
    return "Results Reporter";
}
