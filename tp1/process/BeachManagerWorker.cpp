#include "BeachManagerWorker.h"
#include "../model/Person.h"
#include "../utils/Logger.h"
#include "../constants.h"
#include <sstream>

BeachManagerWorker::BeachManagerWorker(const std::string& fifo_read, const std::string& fifo_write, Semaphore& players_playing)
   : _pipe_writer(fifo_write), _pipe_reader(fifo_read), _i(0), _players_playing(players_playing)  {
}

BeachManagerWorker::~BeachManagerWorker() {
}

void BeachManagerWorker::sendPerson(int i) {
    Person p(i);
    _pipe_writer.escribir(static_cast<void*>(&p), sizeof(Person));
    Logger::log(prettyName(), Logger::INFO, "Enviada persona " + p.id() + " para jugar", Logger::get_date());
}

int BeachManagerWorker::do_work() {
    int j;
    Logger::log(prettyName(), Logger::DEBUG, "Esperando para leer nuevos jugadores", Logger::get_date());
    _pipe_reader.leer(&j, sizeof(int));
    std::stringstream ss;
    ss << "Lei un nuevo jugador: " << j;
    std::string s = ss.str();
    Logger::log(prettyName(), Logger::DEBUG, s, Logger::get_date());

    _i++;
    _players_playing.p();
    Logger::log(prettyName(), Logger::DEBUG, "Semaforo decrementado", Logger::get_date());

    // Si se cumplio que ya quisieron ingresar MIN_PEOPLE personas, mando a todas haciendo que arranque el torneo
    if (_i == MIN_PEOPLE) {
        // Se puede hacer esto porque al no haber arrancado el torneo todavía, los que ingresan sabemos que son del 0 al 9
        for(int i = 0; i < MIN_PEOPLE; i++) {
            sendPerson(i);
        }
    } else {
        // Una vez que ya arranco el torneo (Ya quisieron ingresar mas de MIN_PEOPLE), mando de a una persona
        if (_i > MIN_PEOPLE) {
            sendPerson(j);
        }
    }
    return 0;
}

void BeachManagerWorker::initialize() {
    Logger::log(prettyName(), Logger::DEBUG, "Inicializando", Logger::get_date());
    _pipe_reader.abrir();
    _pipe_writer.abrir();
    Logger::log(prettyName(), Logger::DEBUG, "Pipes inicializados", Logger::get_date());
    Logger::log(prettyName(), Logger::INFO, "Inicializado", Logger::get_date());
}

void BeachManagerWorker::finalize() {
    Logger::log(prettyName(), Logger::DEBUG, "Finalizando", Logger::get_date());
    _pipe_reader.cerrar();
    _pipe_writer.cerrar();
    _pipe_writer.eliminar();
    Logger::log(prettyName(), Logger::DEBUG, "Fin clausura de pipes", Logger::get_date());
    _players_playing.remove();
    Logger::log(prettyName(), Logger::DEBUG, "Semaforo removido", Logger::get_date());
    Logger::log(prettyName(), Logger::INFO, "Finalizado", Logger::get_date());
}

std::string BeachManagerWorker::prettyName() {
    return "Beach Manager Worker";
}
