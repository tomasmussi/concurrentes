//
// Created by tomas on 01/10/17.
//

#include "CourtManager.h"
#include "Team.h"
#include "Match.h"

CourtManager::CourtManager(int m, int k,int rows, int columns, const std::string& fifo_read) :
    _m(m), _k(k), _rows(rows), _columns(columns), _fifo_read(fifo_read) {
}

CourtManager::~CourtManager() {
}

int CourtManager::do_work() {
    // TODO: VERIFICAR QUE HAYA CANCHAS LIBRES PARA DESPACHAR!
    Team team1;
    Team team2;
    _fifo_read.leer(static_cast<void*>(&team1), sizeof(Team));
    Logger::log(prettyName(), Logger::INFO, "Recibido equipo1 " + team1.to_string(), Logger::get_date());
    _fifo_read.leer(static_cast<void*>(&team2), sizeof(Team));
    Logger::log(prettyName(), Logger::INFO, "Recibido equipo2 " + team2.to_string(), Logger::get_date());
    Match match(team1, team2);

    return 0;
}

void CourtManager::initialize() {
    _fifo_read.abrir();
    std::string date = Logger::get_date();
    Logger::log(prettyName(), Logger::DBG, "Fifo de recepcion de equipos abierto", date);
    try {
        _shm_player_couple = new MemoriaCompartida<int>[_m * _k];
        initialize_shm();
        Logger::log(prettyName(), Logger::INFO, "Shared Memory Pareja Personas inicializada", Logger::get_date());
    } catch (const std::string& error) {
        Logger::log(prettyName(), Logger::ERROR, error, Logger::get_date());
    }
}

void CourtManager::finalize() {
    _fifo_read.cerrar();
    destroy_shm();
}

std::string CourtManager::prettyName() {
    return "Court Manager";
}

void CourtManager::initialize_shm() {
    for (int row = 0; row < _m; row++) {
        for (int col = 0; col < _k; col++) {
            // TODO WARNING!!!! NO SE PUEDEN CREAR MAS DE 256 CON ESTO!!!!!!
            _shm_player_couple[get_shm_index(row,col)].crear(__FILE__, row / _m + col);
        }
    }
}

void CourtManager::destroy_shm() {
    for (int row = 0; row < _m; row++) {
        for (int col = 0; col < _k; col++) {
            // TODO WARNING!!!! NO SE PUEDEN CREAR MAS DE 256 CON ESTO!!!!!!
            _shm_player_couple[get_shm_index(row,col)].liberar();
        }
    }
    delete [] _shm_player_couple;
}

int CourtManager::get_shm_index(int row, int col) {
    return row / _m + col;
}