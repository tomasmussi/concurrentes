//
// Created by tomas on 11/09/17.
//

#include "Match.h"
#include "Logger.h"
#include <stdlib.h>
#include <sstream>
#include <unistd.h>


Match::Match(Team team1, Team team2) : __team1(team1), __team2(team2),

    _row(0), _column(0),_probability(0.50), _score_team1(0), _score_team2(0) {
    pid_t pid = fork();
    if (pid > 0) {
        return;
    } else {
        std::string timestamp = Logger::get_date();
        Logger::log("Match", Logger::INFO, __team1.to_string() + " vs "
                 + __team2.to_string() + " por comenzar", timestamp);
        this->run_match();
        Logger::log("Match", Logger::INFO, to_string(), timestamp);
    }

}

// Simular el partido y dar a un equipo como ganador
void Match::run_match() {
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

/**
 * Recordar:
 * 3-0 y 3-1 son 3 puntos ganadores y 0 puntos perdedores
 * 3-2 son 2 puntos ganadores y 1 punto perdedores
 * */


int Match::get_team1_score() const {
    return _score_team1;
}

int Match::get_team2_score() const {
    return _score_team2;
}

std::list<int> Match::get_team1() const {
    return _team1;
}

std::list<int> Match::get_team2() const {
    return _team2;
}

std::string Match::to_string() {
    std::ostringstream stream;
    stream << __team1.to_string() << " vs " << __team2.to_string();
    stream << ": " << _score_team1 << " a " << _score_team2;
    return stream.str();
}