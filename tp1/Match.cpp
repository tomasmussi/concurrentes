//
// Created by tomas on 11/09/17.
//

#include "Match.h"


Match::Match(const std::list<int>& team1, const std::list<int>& team2, int row, int column)
    : _team1(team1), _team2(team2), _row(row), _column(column) {
    // Simular el partido y dar a un equipo como ganador
}