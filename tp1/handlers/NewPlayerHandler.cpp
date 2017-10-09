#include <signal.h>
#include <iostream>
#include <sstream>
#include "NewPlayerHandler.h"

NewPlayerHandler::NewPlayerHandler(const std::string& fifo_write)
        : _pipe_writer(fifo_write), _lock_shm_gone_players("/tmp/shm_gone_users"),
          _shm_gone_players(NULL), _i(0), _last_gone(0) {
}

NewPlayerHandler::~NewPlayerHandler() {
    Logger::log(prettyName(), Logger::DEBUG, "Finalizando", Logger::get_date());
    _pipe_writer.cerrar();
    _pipe_writer.eliminar();
    Logger::log(prettyName(), Logger::DEBUG, "Fifo cerrado", Logger::get_date());
    destroy_shm_gone_players();
    Logger::log(prettyName(), Logger::DEBUG, "SHM de jugadores que se fueron destruida", Logger::get_date());
    Logger::log(prettyName(), Logger::INFO, "Finalizado", Logger::get_date());
}

int NewPlayerHandler::handleSignal(int signum) {
    Logger::log(prettyName(), Logger::INFO, "Nuevo player recibido", Logger::get_date());
    int id = _i < 10 ? _i : read_shm_gone_players() ;
    _pipe_writer.escribir(static_cast<void*>(&id), sizeof(int));
    std::stringstream ss;
    ss << "Escribi en el fifo de nuevas personas: " << id;
    std::string s = ss.str();
    Logger::log(prettyName(), Logger::DEBUG, s, Logger::get_date());
    _i++;
    return 0;
}

void NewPlayerHandler::destroy_shm_gone_players() {
    _lock_shm_gone_players.lock();
    for (int i = 0; i < MAX_GONE_PLAYERS; i++) {
        _shm_gone_players[i].liberar();
    }
    _lock_shm_gone_players.release();
    delete [] _shm_gone_players;
}

int NewPlayerHandler::read_shm_gone_players() {
    _lock_shm_gone_players.lock();
    int id = _shm_gone_players[_last_gone++].leer();
    // Esto no deberia pasar a menos que se hayan ido MAX_GONE_PLAYERS. A partir de ahi vuelve a arrancar
    if(_last_gone == MAX_GONE_PLAYERS) {
        _last_gone = 0;
    }
    _lock_shm_gone_players.release();
    return id;
}

void NewPlayerHandler::initialize() {
    Logger::log(prettyName(), Logger::DEBUG, "Inicializando", Logger::get_date());
    _pipe_writer.abrir();
    Logger::log(prettyName(), Logger::DEBUG, "Pipe inicializado", Logger::get_date());
    initialize_shm_gone_players();
    Logger::log(prettyName(), Logger::DEBUG, "SHM de jugadores que se fueron creada", Logger::get_date());
    Logger::log(prettyName(), Logger::INFO, "Inicializado", Logger::get_date());
}

void NewPlayerHandler::initialize_shm_gone_players() {
    _lock_shm_gone_players.lock();
    _shm_gone_players = new MemoriaCompartida<int>[MAX_GONE_PLAYERS];
    for (int i = 0; i < MAX_GONE_PLAYERS; i++) {
        // TODO WARNING!!!! NO SE PUEDEN CREAR MAS DE 256 CON ESTO!!!!!!
        _shm_gone_players[i].crear("/bin/bash", i);
    }
    _lock_shm_gone_players.release();
}

std::string NewPlayerHandler::prettyName() {
    return "New Player Handler";
}
