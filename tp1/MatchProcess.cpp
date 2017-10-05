//
// Created by tomas on 11/09/17.
//

#include "MatchProcess.h"
#include "Logger.h"
#include <stdlib.h>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <signal.h>


MatchProcess::MatchProcess(pid_t parent_process_id) : _father_id(parent_process_id),
    _probability(0.5), _score_team1(0), _score_team2(0),
    _lock_matches("/tmp/shm_matches"), _shm_matches(NULL) {
}

MatchProcess::~MatchProcess() {
    if (_shm_matches != NULL) {
        Logger::log(prettyName(), Logger::DBG, "Destruyendo SHM", Logger::get_date());
        _shm_matches->liberar();
    } else {
        Logger::log(prettyName(), Logger::DBG, "No toco shm porque es null", Logger::get_date());
    }
}


/**
 * Al llamar a esta funcion, se pasa a desarrollar un partido en un proceso aparte
 * Si no se llama a esta funcion, se puede utilizar el objeto como contenedor de
 * informacion
 * */
void MatchProcess::dispatch_match() {
    Logger::log(prettyName(), Logger::DBG, "Creando memoria compartida de matches", Logger::get_date());
    _shm_matches = new MemoriaCompartida<int>;
    _shm_matches->crear("/bin/grep", 'a');

    this->run_match();

    std::string timestamp = Logger::get_date();
    signal_court_manager();
    Logger::log(prettyName(), Logger::INFO, "CourtManager senializado fin partido", timestamp);
    Logger::log(prettyName(), Logger::DBG, "Ahora deberia venir el dt de SHM", timestamp);
    _exit(get_match_result());
}

// Simular el partido y dar a un equipo como ganador
void MatchProcess::run_match() {
    std::srand(std::time(NULL));
    int who_wins = rand() % 100;
    int prob = 100 * _probability;
    if (prob >= who_wins) {
        // team1 wins
        set_scores(_score_team1, _score_team2);
    } else {
        // team2 wins
        set_scores(_score_team2, _score_team1);
    }
}

void MatchProcess::set_scores(int& score_winner, int& score_loser) {
    score_winner = 3;
    int prob = 100 * _probability;
    int wins_by_difference = rand() % 100;
    if (prob >= wins_by_difference) {
        // by 3-0
        score_loser = 0;
    } else {
        int final_score = rand() % 100;
        if (MATCH_PROBABILITY >= final_score) {
            score_loser = 1;
        } else {
            score_loser = 2;
        }
    }
}

void MatchProcess::signal_court_manager() {
    Logger::log(prettyName(), Logger::DBG, "Tomando lock de matches", Logger::get_date());
    _lock_matches.lock();
    _shm_matches->escribir(_shm_matches->leer() + 1);
    _lock_matches.release();
    Logger::log(prettyName(), Logger::DBG, "Lock de matches liberado", Logger::get_date());
    kill(_father_id, SIGUSR1);
}

/**
 * Recordar:
 * 3-0 y 3-1 son 3 puntos ganadores y 0 puntos perdedores
 * 3-2 son 2 puntos ganadores y 1 punto perdedores
 * 0 : Termino 3-0
 * 1 : Termino 3-1
 * 2 : Termino 3-2
 * 3 : Termino 2-3
 * 4 : Termino 1-3
 * 5 : Termino 0-3
 * 6 : No termino
 * */
int MatchProcess::get_match_result() {
    if (_score_team1 == 3 && _score_team2 == 0) {
        return 0;
    } else if (_score_team1 == 3 && _score_team2 == 1) {
        return 1;
    } else if (_score_team1 == 3 && _score_team2 == 2) {
        return 2;
    } else if (_score_team1 == 2 && _score_team2 == 3) {
        return 3;
    } else if (_score_team1 == 1 && _score_team2 == 3) {
        return 4;
    } else if (_score_team1 == 0 && _score_team2 == 3) {
        return 5;
    }
    return 6;
}

std::string MatchProcess::prettyName() {
    std::stringstream ss;
    ss << "Match Process (" << getpid() << ")";
    return ss.str();
}


/*
MatchProcess::MatchProcess(const MatchProcess& other_match) : _lock_matches("/tmp/shm_matches"), _son_process(false) {
    Logger::log("MatchProcess", Logger::DBG, "Construyendo COPIA", Logger::get_date());
    this->_team1 = other_match._team1;
    this->_team2 = other_match._team2;
    this->_score_team1 = other_match._score_team1;
    this->_score_team2 = other_match._score_team2;
}

MatchProcess MatchProcess::operator=(const MatchProcess& other_match) {
    this->_team1 = other_match._team1;
    this->_team2 = other_match._team2;
    this->_score_team1 = other_match._score_team1;
    this->_score_team2 = other_match._score_team2;
    this->_son_process = false;
    return *this;
}
 */