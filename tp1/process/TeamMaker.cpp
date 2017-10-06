#include "TeamMaker.h"
#include "../utils/Logger.h"
#include "../model/Team.h"

#include <cstring>

TeamMaker::TeamMaker(int m, int k, const std::string& fifo_read, const std::string& fifo_write)
        : _m(m), _k(k), _fifo_read(fifo_read), _fifo_write(fifo_write),
          _lock_shm_mapper("/tmp/shm_mapper"), _shm_mapper(NULL),
          _lock_shm_player_couple("/tmp/shm_player_couple"), _shm_player_couple(NULL),
          _have_player(false), _id_mapper(), _waiting_list() {
}

int TeamMaker::lookup(const Person& person) {
    int idx = -1;
    _lock_shm_mapper.lock();
    for (int i = 0; i < _m; i++) {
        int id = _shm_mapper[i].leer();
        if (person.is(id)){
            idx = i;
        }
    }
    _lock_shm_mapper.release();
    return idx;
}

int TeamMaker::find_empty_space(const Person& person) {
    int idx = -1;
    _lock_shm_mapper.lock();
    for (int i = 0; i < _m; i++) {
        int id = _shm_mapper[i].leer();
        if (id == -1) {
            _shm_mapper[i].escribir(person.int_id());
            idx = i;
            break;
        }
    }
    _lock_shm_mapper.release();
    return idx;
}

bool TeamMaker::can_play(int shm_id, const Person& p) {
    int shm_other_id = lookup(p);
    _lock_shm_player_couple.lock();
    for (int col = 0; col < _k; col++) {
        int shm_iter_id = _shm_player_couple[get_shm_index(shm_id, col)].leer();
        if (shm_other_id == shm_iter_id) {
            // Esto deberia verse al menos una vez, para ver que la SHM funciona
            Logger::log(prettyName(), Logger::DEBUG, "Ya jugaron juntos", Logger::get_date());
            return false;
        }
    }
    _lock_shm_player_couple.release();
    return true;
}

int TeamMaker::do_work() {
    Person p1(-1);
    ssize_t read = _fifo_read.leer(static_cast<void*>(&p1), sizeof(Person));
    std::string timestamp = Logger::get_date();
    if (read && p1.valid()) {
        Logger::log(prettyName(), Logger::INFO, "Llego persona de id: " + p1.id(),timestamp);
        int shm_id = lookup(p1);

        if (shm_id == -1) {
            // No estaba registrado en la shm
            shm_id = find_empty_space(p1);
        }
        if (shm_id == -1) {
            // Esto no deberia pasar jamas porque el BeachManagerWorker no deberia mandar mas que _m
            Logger::log(prettyName(), Logger::ERROR, "SHM MAPPER INDEX -1 EN TEAM MAKER", Logger::get_date());
        }
        if (_waiting_list.size() > 0) {
//            bool could_play = false;
            for (std::list<Person>::iterator it = _waiting_list.begin(); it != _waiting_list.end(); ++it) {
                if (can_play(shm_id, *it)) {
//                    could_play = true;
                    Team team(*it, p1);
                    _fifo_write.escribir(static_cast<void*>(&team), sizeof(Team));
                    Logger::log(prettyName(), Logger::INFO, "Enviando equipo: " + team.to_string(), Logger::get_date());
                    _waiting_list.erase(it);
                    break;
                }
            }
//            if (!could_play) {
//                _waiting_list.push_back(p1);
//            }
        } else {
            _waiting_list.push_back(p1);
        }
    }
    return 0;
}

void TeamMaker::initialize() {
    Logger::log(prettyName(), Logger::DEBUG, "Inicializando" ,Logger::get_date());
    _fifo_read.abrir();
    Logger::log(prettyName(), Logger::DEBUG, "Fifo de recepcion de nuevas personas abierto" ,Logger::get_date());

    try {
        initialize_shm();
        Logger::log(prettyName(), Logger::DEBUG, "Shared Memory Pareja Personas inicializada", Logger::get_date());
    } catch (const std::string& error) {
        Logger::log(prettyName(), Logger::ERROR, error, Logger::get_date());
    }

    try {
        initalize_shm_mapper();
        Logger::log(prettyName(), Logger::DEBUG, "Shared Memory Mapper Personas inicializada", Logger::get_date());
    } catch (const std::string& error) {
        Logger::log(prettyName(), Logger::ERROR, error, Logger::get_date());
    }

    _fifo_write.abrir();
    Logger::log(prettyName(), Logger::DEBUG, "Fifo de WRITE de equipos a CourtManager abierto", Logger::get_date());
    Logger::log(prettyName(), Logger::INFO, "Inicializado", Logger::get_date());
}

void TeamMaker::finalize() {
    Logger::log(prettyName(), Logger::DEBUG, "Finalizando", Logger::get_date());
    _fifo_read.cerrar();
    _fifo_write.cerrar();
    _fifo_write.eliminar();
    Logger::log(prettyName(), Logger::DEBUG, "Fifos cerrados", Logger::get_date());

    destroy_shm();
    Logger::log(prettyName(), Logger::DEBUG, "SHM player couple destruida", Logger::get_date());
    destroy_shm_mapper();
    Logger::log(prettyName(), Logger::DEBUG, "SHM mapper destruida", Logger::get_date());
    Logger::log(prettyName(), Logger::INFO, "Finalizado", Logger::get_date());
}

std::string TeamMaker::prettyName() {
    return "Team Maker";
}

void TeamMaker::initialize_shm() {
    _lock_shm_player_couple.lock();
    _shm_player_couple = new MemoriaCompartida<int>[_m * _k];
    for (int row = 0; row < _m; row++) {
        for (int col = 0; col < _k; col++) {
            // TODO WARNING!!!! NO SE PUEDEN CREAR MAS DE 256 CON ESTO!!!!!!
            _shm_player_couple[get_shm_index(row,col)].crear(__FILE__, get_shm_index(row,col));
            _shm_player_couple[get_shm_index(row,col)].escribir(-1);
        }
    }
    _lock_shm_player_couple.release();
}

void TeamMaker::destroy_shm() {
    _lock_shm_player_couple.lock();
    for (int row = 0; row < _m; row++) {
        for (int col = 0; col < _k; col++) {
            // TODO WARNING!!!! NO SE PUEDEN CREAR MAS DE 256 CON ESTO!!!!!!
            _shm_player_couple[get_shm_index(row,col)].liberar();
        }
    }
    _lock_shm_player_couple.release();
    delete [] _shm_player_couple;
}

void TeamMaker::initalize_shm_mapper() {
    _lock_shm_mapper.lock();
    _shm_mapper = new MemoriaCompartida<int>[_m];
    for (int i = 0; i < _m; i++) {
        // TODO WARNING!!!! NO SE PUEDEN CREAR MAS DE 256 CON ESTO!!!!!!
        _shm_mapper[i].crear("/bin/cat", i);
        _shm_mapper[i].escribir(-1);
    }
    _lock_shm_mapper.release();
}

void TeamMaker::destroy_shm_mapper() {
    _lock_shm_mapper.lock();
    for (int i = 0; i < _m; i++) {
        // TODO WARNING!!!! NO SE PUEDEN CREAR MAS DE 256 CON ESTO!!!!!!
        _shm_mapper[i].liberar();
    }
    delete [] _shm_mapper;
    _lock_shm_mapper.release();
}

int TeamMaker::get_shm_index(int row, int col) {
    return row / _m + col;
}