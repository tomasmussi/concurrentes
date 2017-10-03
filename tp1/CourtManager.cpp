//
// Created by tomas on 01/10/17.
//
#include <sys/wait.h>
#include <sstream>

#include "CourtManager.h"
#include "Team.h"
#include "Logger.h"
#include "SignalHandler.h"

CourtManager::CourtManager(int m, int k,int rows, int columns, const std::string& fifo_read,
        const std::string& fifo_write) :
    _m(m), _k(k), _rows(rows), _columns(columns), _fifo_read(fifo_read), _fifo_write(fifo_write),
    _shm_player_couple(NULL) {
}

CourtManager::~CourtManager() {
}

int CourtManager::do_work() {
    // TODO: VERIFICAR QUE HAYA CANCHAS LIBRES PARA DESPACHAR!
    Team team1;
    Team team2;
    _fifo_read.leer(static_cast<void*>(&team1), sizeof(Team));
    if (team1.valid()) {
        Logger::log(prettyName(), Logger::INFO, "Recibido equipo1 " + team1.to_string(), Logger::get_date());
        _fifo_read.leer(static_cast<void*>(&team2), sizeof(Team));
        if (team2.valid()) {
            Logger::log(prettyName(), Logger::INFO, "Recibido equipo2 " + team2.to_string(), Logger::get_date());
            Match match(team1, team2, getpid());
            pid_t pid = match.dispatch_match();
            _matches[pid] = match;

            Match prueba = _matches[pid];
            Logger::log(prettyName(), Logger::DBG, prueba.to_string() + " deberia tener: " + team1.to_string() + " + " + team2.to_string(), Logger::get_date());
        }
    }
    return 0;
}

void CourtManager::initialize() {
    _fifo_read.abrir();
    std::string date = Logger::get_date();
    Logger::log(prettyName(), Logger::DBG, "Fifo READ de equipos de TeamMaker", date);
    _fifo_write.abrir();
    Logger::log(prettyName(), Logger::DBG, "Fifo de envio de personas a TeamMaker", Logger::get_date());
    try {
        initialize_shm();
        Logger::log(prettyName(), Logger::INFO, "Shared Memory Pareja Personas inicializada", Logger::get_date());
    } catch (const std::string& error) {
        Logger::log(prettyName(), Logger::ERROR, error, Logger::get_date());
    }
    try {
        initalize_shm_mapper();
        Logger::log(prettyName(), Logger::INFO, "Shared Memory Mapper", Logger::get_date());
    } catch (const std::string& error) {
        Logger::log(prettyName(), Logger::ERROR, error, Logger::get_date());
    }
    SignalHandler::getInstance()->registrarHandler(SIGUSR1, this);
}

void CourtManager::finalize() {
    _fifo_read.cerrar();
    _fifo_write.cerrar();
    destroy_shm();
    destroy_shm_mapper();
    SignalHandler::destroy();
}

std::string CourtManager::prettyName() {
    return "Court Manager";
}

void CourtManager::initialize_shm() {
    _shm_player_couple = new MemoriaCompartida<int>[_m * _k];
    for (int row = 0; row < _m; row++) {
        for (int col = 0; col < _k; col++) {
            // TODO WARNING!!!! NO SE PUEDEN CREAR MAS DE 256 CON ESTO!!!!!!
            _shm_player_couple[get_shm_index(row,col)].crear(__FILE__, get_shm_index(row,col));
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

void CourtManager::initalize_shm_mapper() {
    _shm_mapper = new MemoriaCompartida<int>[_m];
    for (int i = 0; i < _m; i++) {
        // TODO WARNING!!!! NO SE PUEDEN CREAR MAS DE 256 CON ESTO!!!!!!
        _shm_mapper[i].crear("/bin/cat", i);
    }
}

void CourtManager::destroy_shm_mapper() {
    for (int i = 0; i < _m; i++) {
        // TODO WARNING!!!! NO SE PUEDEN CREAR MAS DE 256 CON ESTO!!!!!!
        _shm_mapper[i].liberar();
    }
    delete [] _shm_player_couple;
}

int CourtManager::get_shm_index(int row, int col) {
    return row / _m + col;
}

int CourtManager::lookup(const Person& person) {
    int idx = -1;
    for (int i = 0; i < _m; i++) {
        int id = _shm_mapper[i].leer();
        if (person.is(id)){
            idx = i;
        }
    }
    return idx;
}

void CourtManager::write_shm_mapper(int idx_p1, int idx_p2) {
    for (int col = 0; col < _k; col++) {
        int idx = _shm_player_couple[get_shm_index(idx_p1, col)].leer();
        if (idx == -1) {
            _shm_player_couple[get_shm_index(idx_p1, col)].escribir(idx_p2);
            break;
        }
    }
    for (int col = 0; col < _k; col++) {
        int idx = _shm_player_couple[get_shm_index(idx_p2, col)].leer();
        if (idx == -1) {
            _shm_player_couple[get_shm_index(idx_p2, col)].escribir(idx_p1);
            break;
        }
    }
}

int CourtManager::handleSignal ( int signum ) {
    if (signum != SIGUSR1) {
        Logger::log(prettyName(), Logger::ERROR, "Recibi senial distinta a SIGUSR1", Logger::get_date());
        return 1;
    }
    int status;
    pid_t match_pid = wait(&status);
    Match match = _matches[match_pid];
    match.set_match_status(WEXITSTATUS(status));
    _matches[match_pid] = match;
    std::stringstream ss;
    ss << "Match [" << match_pid << "] ";
    if (match.finished()) {
        ss << "finalizo " << match.to_string();
        Logger::log(prettyName(), Logger::INFO, ss.str(), Logger::get_date());
        // El partido termino, por lo que hay que escribir el resultado
        // TODO Aca se deberia escribir una memoria compartida para que los reporters puedan leer resultados

        // Escribir la shm de los equipos que jugaron en parejas
        int team_idx_1 = lookup(match.get_team1().get_person1());
        int team_idx_2 = lookup(match.get_team1().get_person2());
        write_shm_mapper(team_idx_1, team_idx_2);

        team_idx_1 = lookup(match.get_team2().get_person1());
        team_idx_2 = lookup(match.get_team2().get_person2());
        write_shm_mapper(team_idx_1, team_idx_2);

    } else {
        ss << "inundado " << match.to_string();
        Logger::log(prettyName(), Logger::INFO, ss.str(), Logger::get_date());
    }
    // Envio a Team Maker T1-P1; T2-P2; T1-P2; T2-P1
    Person p;
    p = match.get_team1().get_person1();
    _fifo_write.escribir(static_cast<void*>(&p), sizeof(Person));
    Logger::log(prettyName(), Logger::INFO, "Enviada persona a TeamMaker", Logger::get_date());
    p = match.get_team2().get_person2();
    _fifo_write.escribir(static_cast<void*>(&p), sizeof(Person));
    Logger::log(prettyName(), Logger::INFO, "Enviada persona a TeamMaker", Logger::get_date());
    p = match.get_team1().get_person2();
    _fifo_write.escribir(static_cast<void*>(&p), sizeof(Person));
    Logger::log(prettyName(), Logger::INFO, "Enviada persona a TeamMaker", Logger::get_date());
    p = match.get_team2().get_person1();
    _fifo_write.escribir(static_cast<void*>(&p), sizeof(Person));
    Logger::log(prettyName(), Logger::INFO, "Enviada persona a TeamMaker", Logger::get_date());
    return 0;
}