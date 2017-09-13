//
// Created by tomas on 11/09/17.
//

#include "Match.h"

/**
 * TODO Hay que hacer toda la simulacion y probablemente buscar una forma mejor de mostrar resultados
 * */

Match::Match(const std::list<int>& team1, const std::list<int>& team2, int row, int column)
    : _team1(team1), _team2(team2), _row(row), _column(column) {
    // Simular el partido y dar a un equipo como ganador
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
    return 2;
}

int Match::get_team2_score() const {
    return 1;
}

std::list<int> Match::get_team1() const {
    return _team1;
}

std::list<int> Match::get_team2() const {
    return _team2;
}