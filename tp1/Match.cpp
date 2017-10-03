//
// Created by tomas on 11/09/17.
//

#include "Match.h"
#include "Logger.h"
#include <stdlib.h>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <signal.h>

Match::Match() {
}

Match::Match(Team team1, Team team2, pid_t pid) :
    _team1(team1), _team2(team2),
    _probability(0.5), _score_team1(0), _score_team2(0),
    _court_manager_pid(pid) {
}

Match::Match(const Match& other_match) {
    this->_team1 = other_match._team1;
    this->_team2 = other_match._team2;
    this->_score_team1 = other_match._score_team1;
    this->_score_team2 = other_match._score_team2;
}

Match Match::operator=(const Match& other_match) {
    this->_team1 = other_match._team1;
    this->_team2 = other_match._team2;
    this->_score_team1 = other_match._score_team1;
    this->_score_team2 = other_match._score_team2;
    return *this;
}

/**
 * Al llamar a esta funcion, se pasa a desarrollar un partido en un proceso aparte
 * Si no se llama a esta funcion, se puede utilizar el objeto como contenedor de
 * informacion
 * */
pid_t Match::dispatch_match() {
    pid_t pid = fork();
    if (pid > 0) {
        return pid;
    }
    this->run_match();
    Logger::log("Match", Logger::INFO, to_string(), Logger::get_date());

    std::string timestamp = Logger::get_date();
    signal_court_manager();
    std::stringstream ss;
    ss << "[" << getpid() << "] CourtManager senializado fin partido";
    Logger::log("Match", Logger::INFO, ss.str(), timestamp);
    _exit(get_match_result());
}



// Simular el partido y dar a un equipo como ganador
void Match::run_match() {
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

void Match::set_scores(int& score_winner, int& score_loser) {
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

std::string Match::to_string() {
    std::ostringstream stream;
    stream << "[" << getpid() << "] " << _team1.to_string() << " vs " << _team2.to_string();
    stream << ": " << _score_team1 << " a " << _score_team2;
    return stream.str();
}

void Match::signal_court_manager() {
    kill(_court_manager_pid, SIGUSR1);
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
int Match::get_match_result() {
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

void Match::set_match_status(int exit_code) {
    switch (exit_code) {
        case 0:
            _score_team1 = 3;
            _score_team2 = 0;
            break;
        case 1:
            _score_team1 = 3;
            _score_team2 = 1;
            break;
        case 2:
            _score_team1 = 3;
            _score_team2 = 2;
            break;
        case 3:
            _score_team1 = 2;
            _score_team2 = 3;
            break;
        case 4:
            _score_team1 = 1;
            _score_team2 = 3;
            break;
        case 5:
            _score_team1 = 0;
            _score_team2 = 3;
            break;
        case 6:
            _score_team1 = 0;
            _score_team2 = 0;
            break;
    };
}

bool Match::finished() {
    return _score_team1 != 0 || _score_team2 != 0;
}

Team Match::get_team1() const {
    return _team1;
}

Team Match::get_team2() const {
    return _team2;
}
