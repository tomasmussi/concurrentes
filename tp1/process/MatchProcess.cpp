#include "MatchProcess.h"
#include "../constants.h"
#include <stdlib.h>
#include <cstdlib>
#include <signal.h>
#include <sstream>

MatchProcess::MatchProcess(pid_t parent_process_id) : _father_id(parent_process_id),
    _probability(0.5), _score_team1(0), _score_team2(0) {
}

MatchProcess::~MatchProcess() {
    finalize();
}

void MatchProcess::finalize() {
    Logger::log(prettyName(), Logger::INFO, "Finalizado", Logger::get_date());
}

// Simular el partido y dar a un equipo como ganador
void MatchProcess::run_match() {
    srand(static_cast<unsigned int>(time(0)+getpid())); //getpid evita que dos partidos que arrancan al mismo tiempo, tengan el mismo resultado
    int play_time = rand() % 10;
    // Simulo que el partido dura un tiempo aleatorio entre 0 y 9 segundos
    sleep(1);
    int who_wins = rand() % 100;
    int prob = 100 * _probability;
    if (prob >= who_wins) {
        // team1 wins
        set_scores(_score_team1, _score_team2);
    } else {
        // team2 wins
        set_scores(_score_team2, _score_team1);
    }
    std::stringstream ss;
    ss << "El partido duro " << play_time << " segundos y termino: " << _score_team1 << " a " << _score_team2;
    Logger::log(prettyName(), Logger::INFO, ss.str(), Logger::get_date());
}

void MatchProcess::set_scores(int& score_winner, int& score_loser) {
    score_winner = 3;
    int random = rand() % 100;
    if (random < 33) {
        score_loser = 0;
    }
    else if (random < 66) {
        score_loser = 1;
    }
    else {
        score_loser = 2;
    }
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
    Logger::log("MatchProcess", Logger::DEBUG, "Construyendo COPIA", Logger::get_date());
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
