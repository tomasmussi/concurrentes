//
// Created by tomas on 11/09/17.
//

#include "Match.h"
#include <stdlib.h>

/**
 * TODO Hay que hacer toda la simulacion y probablemente buscar una forma mejor de mostrar resultados
 * */
Match::Match(const std::list<int>& team1, const std::list<int>& team2, int row, int column)
    : _team1(team1), _team2(team2), _row(row), _column(column),_probability(1),
      _score_team1(0), _score_team2(0) {
    this->run_match();
}

Match::Match(const std::list<int>& team1, const std::list<int>& team2, int row, int column,
    float probability) : _team1(team1), _team2(team2), _row(row), _column(column),
    _probability(probability), _score_team1(0), _score_team2(0) {
    if (_probability > 1 || _probability < 0) {
        _probability = 1;
    }
    this->run_match();
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
std::map<int, int> Match::get_individual_scores() {
    std::map<int, int> points;
    std::list<int>::iterator it;

    // Team 1 gana 3-2
    for (it = _team1.begin(); it != _team1.end(); ++it) {
        points[*it] = get_team1_score();
    }
    for (it = _team2.begin(); it != _team2.end(); ++it) {
        points[*it] = get_team2_score();
    }
    return points;
}

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