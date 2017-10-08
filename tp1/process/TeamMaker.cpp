#include "TeamMaker.h"
#include "../model/Team.h"
#include "../utils/Logger.h"

#include <cstring>
#include <sstream>

TeamMaker::TeamMaker(int k, const std::string& fifo_read, const std::string& fifo_write, Semaphore& semaphore)
        : _k(k), _fifo_read(fifo_read), _fifo_write(fifo_write), _semaphore(semaphore),
          _have_player(false), _couples(), _waiting_list() {
}

bool TeamMaker::can_play(std::string id_p1, std::string id_p2) {
    return !_couples[id_p1][id_p2];
}

void TeamMaker::new_id_into_map(std::string id){
    for (std::map<std::string,std::map<std::string,bool> > ::iterator it = _couples.begin(); it != _couples.end(); it++ ){
        _couples[id][it->first] = false;
        _couples[it->first][id] = false;
    }
    _couples[id][id] = true; // IMPORTANTE: Para asegurar que con el primero que entre se inicialice el map. Sino, nunca se agrega nadie
}

void TeamMaker::add_couple_to_map(std::string id_p1, std::string id_p2){
    _couples[id_p1][id_p2] = true;
    _couples[id_p2][id_p1] = true;
}

int TeamMaker::matches_played(std::string id) {
    int count = 0;
    std::map<std::string, bool> asd = _couples[id];
    for(std::map<std::string, bool>::iterator it = asd.begin(); it != asd.end(); it++) {
        if (asd[it->first] and it->first != id) {
            count++;
        }
    }
    std::stringstream ss;
    ss << "La persona con id " << id << " jugo " << count << " partidos";
    Logger::log(prettyName(), Logger::DEBUG, ss.str(), Logger::get_date());
    return count;
}

int TeamMaker::do_work() {
    Person p1(-1);
    ssize_t read = _fifo_read.leer(static_cast<void*>(&p1), sizeof(Person));
    std::string timestamp = Logger::get_date();
    if (read && p1.valid()) {
        std::string id_p1 = p1.id();
        Logger::log(prettyName(), Logger::INFO, "Llego persona de id: " + id_p1, timestamp);

        if (_couples.find(id_p1) == _couples.end()){
            // NO esta registrado (nuevo jugador)
            new_id_into_map(id_p1);
        // Asi no reviso que tenga haya jugado menos que k cuando sabemos que no jugo nunca
        } else {
            if (matches_played(id_p1) >= _k) {
                _semaphore.v();
                Logger::log(prettyName(), Logger::DEBUG, "Aumentando semaforo", Logger::get_date());
                return 0;
            }
        }

        // TODO: Agregar que acá la persona se puede ir. Habria que escribir en una SHM que esa persona se fue y puede volver, asi el NewPlayerHandler lo puede volver a mandar

        bool played = false;
        if (_waiting_list.size() > 0) {
            for (std::list<Person>::iterator it = _waiting_list.begin(); it != _waiting_list.end(); ++it) {
                if (can_play(id_p1, it->id())) {
                    Team team(*it, p1);
                    _fifo_write.escribir(static_cast<void*>(&team), sizeof(Team));
                    add_couple_to_map(id_p1, it->id());
                    Logger::log(prettyName(), Logger::INFO, "Enviando equipo: " + team.to_string(), Logger::get_date());
                    _waiting_list.erase(it);
                    played = true;
                    break;
                }
            }
            if(! played){
                _waiting_list.push_back(p1); // CLAVE: Si no puede jugar contra nadie ahora, lo guarda igual
            }
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
    _semaphore.remove();
    Logger::log(prettyName(), Logger::DEBUG, "Semaforo removido", Logger::get_date());
    Logger::log(prettyName(), Logger::INFO, "Finalizado", Logger::get_date());
}

std::string TeamMaker::prettyName() {
    return "Team Maker";
}
