#include <signal.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include "NewPlayerHandler.h"

NewPlayerHandler::NewPlayerHandler(const std::string& fifo_write)
        : _pipe_writer(fifo_write), _lock_shm_gone_players(SHM_GONE_PLAYERS_LOCK),
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
    Logger::log(prettyName(), Logger::INFO, "Nuevo jugador recibido", Logger::get_date());
    bool old_player = false;
    int id = -1;
    int prob = rand() % 100;
    // Me fijo si quiere entrar uno de los players que se fue
    if (prob < GONE_PLAYER_PROBABILITY) {
        Logger::log(prettyName(), Logger::DEBUG, "Leyendo new player de la SHM de jugadores que se fueron", Logger::get_date());
        id = read_shm_gone_players();
        if (id != -1) {
            old_player = true;
        }
    }
    if (not old_player) {
        Logger::log(prettyName(), Logger::DEBUG, "No habia jugadores que se hayan ido, por lo que el nuevo es realmente un nuevo jugador", Logger::get_date());
        id = _i;
        _i++;
    }
    _pipe_writer.escribir(static_cast<void*>(&id), sizeof(int));
    std::stringstream ss;
    ss << "Enviando jugador al predio: " << id;
    std::string s = ss.str();
    Logger::log(prettyName(), Logger::DEBUG, s, Logger::get_date());
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
    int id = _shm_gone_players[_last_gone].leer();
    // Si lei un id valido...
    if (id != -1) {
        _last_gone++;
        // Esto no deberia pasar a menos que se hayan ido MAX_GONE_PLAYERS. A partir de ahi vuelve a arrancar
        if (_last_gone == MAX_GONE_PLAYERS) {
            _last_gone = 0;
        }
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
        _shm_gone_players[i].crear(SHM_GONE_PLAYERS, i);
    }
    _lock_shm_gone_players.release();
}

std::string NewPlayerHandler::prettyName() {
    return "New Player Handler";
}
