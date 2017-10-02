//
// Created by tomas on 29/09/17.
//

#include "TeamMaker.h"
#include "Logger.h"
#include "Team.h"

#include <cstring>
#include <sstream>

TeamMaker::TeamMaker(int m, int k, const std::string& fifo_read, const std::string& fifo_write)
        : _m(m), _k(k), _fifo_read(fifo_read), _fifo_write(fifo_write),
          _shm_player_couple(NULL), _some_player(0), _have_player(false),
          _id_mapper(), _free_ids(), _waiting_list(),

          _fifo_prueba(fifo_read) { // TODO ELIMINAR
}

int TeamMaker::do_work() {
    Person p1(-1);
    ssize_t read = _fifo_read.leer(static_cast<void*>(&p1), sizeof(Person));
    std::string timestamp = Logger::get_date();
    if (read && p1.valid()) {
        Logger::log(prettyName(), Logger::INFO, "Llego persona de id: " + p1.id(),timestamp);
        int mapped_id;
        if (_id_mapper.find(p1.id()) != _id_mapper.end()) {
            // Estaba registrado en la shm
            mapped_id = _id_mapper[p1.id()];
        } else {
            // Nuevo
            mapped_id = _free_ids.front();
            _free_ids.pop_front();
            _id_mapper[p1.id()] = mapped_id;
        }
        if (_waiting_list.size() > 0) {
            std::list<Person> couple(_waiting_list);
            for (std::list<Person>::iterator it = couple.begin(); it != couple.end(); ++it) {
                int col = _id_mapper[it->id()];
                int played_together = _shm_player_couple[get_shm_index(mapped_id, col)].leer();
                if (played_together == 0) {
                    Team team(p1, *it);
                    std::string timestamp = Logger::get_date();
                    _fifo_write.escribir(static_cast<void*>(&team), sizeof(Team));
                    Logger::log(prettyName(), Logger::INFO, "Enviando equipo: " + team.to_string(), timestamp);

                    timestamp = Logger::get_date();
                    _waiting_list.erase(it);
                    break;
                }
            }
        } else {
            _waiting_list.push_back(p1);
        }
    }
    return 0;
}

void TeamMaker::initialize() {
    _fifo_read.abrir();
    Logger::log(prettyName(), Logger::DBG, "Fifo de recepcion de nuevas personas abierto" ,Logger::get_date());

    try {
        _shm_player_couple = new MemoriaCompartida<int>[_m * _k];
        initialize_shm();
        Logger::log(prettyName(), Logger::DBG, "Shared Memory Pareja Personas inicializada", Logger::get_date());
    } catch (const std::string& error) {
        Logger::log(prettyName(), Logger::ERROR, error, Logger::get_date());
    }
    for (int i = 0; i < _m; i++) {
        _free_ids.push_back(i);
    }
    _fifo_write.abrir();
    Logger::log(prettyName(), Logger::DBG, "Fifo de emision de equipos abierto", Logger::get_date());
}

void TeamMaker::finalize() {
    _fifo_read.cerrar();
    _fifo_write.cerrar();
    _fifo_write.eliminar();

    destroy_shm();

    _fifo_prueba.cerrar();
}

std::string TeamMaker::prettyName() {
    return "Team Maker";
}

void TeamMaker::initialize_shm() {
    for (int row = 0; row < _m; row++) {
        for (int col = 0; col < _k; col++) {
            // TODO WARNING!!!! NO SE PUEDEN CREAR MAS DE 256 CON ESTO!!!!!!
            _shm_player_couple[get_shm_index(row,col)].crear(__FILE__, get_shm_index(row,col));
        }
    }
}

void TeamMaker::destroy_shm() {
    for (int row = 0; row < _m; row++) {
        for (int col = 0; col < _k; col++) {
            // TODO WARNING!!!! NO SE PUEDEN CREAR MAS DE 256 CON ESTO!!!!!!
            _shm_player_couple[get_shm_index(row,col)].liberar();
        }
    }
    delete [] _shm_player_couple;
}

int TeamMaker::get_shm_index(int row, int col) {
    return row / _m + col;
}
