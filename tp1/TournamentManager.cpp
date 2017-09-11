//
// Created by tomas on 11/09/17.
//
#include <iostream>
#include <algorithm>
#include "TournamentManager.h"
#include "Subscriber.h"

TournamentManager::TournamentManager(int k, int rows, int columns) :
    _k(k), _rows(rows), _columns(columns), _started(false), _people(), _tournament_courts() {
    for (int row = 0; row < _rows; row++) {
        for (int col = 0; col < _columns; col++) {
            _tournament_courts[row][col] = 0;
        }
    }
}

void TournamentManager::start_competition(const std::list<int> & subscribed_people) {
    if (subscribed_people.size() < MIN_SUBSCRIBERS) {
        return;
    }
    std::copy(subscribed_people.begin(), subscribed_people.end(),
             std::back_inserter(_people));
    _started = true;
}

void TournamentManager::add_competitor(const int person) {
    _people.push_back(person);
}

bool TournamentManager::started() {
    return _started;
}

/*
 * TODO: Definir cuando es que el torneo termino
 * Creo que es cuando no se pueden armar mas parejas
 * */
bool TournamentManager::finished() {
    return false;
}



void TournamentManager::do_job() {
    std::list<int> team = get_team();
    std::list<int> oponents = get_team_for(team);
    // dispatch teams
}

std::list<int> TournamentManager::get_team() {
    std::list<int> team;
    for (int person = 0; person < _people.size(); person++) {
        for (int couple = 0; couple < _people.size(); couple++) {
            if (person != couple) {
                if (_pairs[person].find(couple) == _pairs[person].end()) {
                    _pairs[person][couple] = 1;
                    _pairs[couple][person] = 1;
                    team.push_back(person);
                    team.push_back(couple);
                    return team;
                }
            }
        }
    }
    return team;
}

bool TournamentManager::in_team(const std::list<int>& team, int person) {
    return std::find(team.begin(), team.end(), person) != team.end();
}

std::list<int> TournamentManager::get_team_for(const std::list<int> & team) {
    std::list<int> other_team;
    for (int person = 0; person < _people.size(); person++) {
        if (in_team(team, person)) {
            continue;
        }
        for (int couple = 0; couple < _people.size(); couple++) {
            if (person == couple || in_team(team, couple)) {
                continue;
            }
            if (_pairs[person].find(couple) == _pairs[person].end()) {
                _pairs[person][couple] = 1;
                _pairs[couple][person] = 1;
                other_team.push_back(person);
                other_team.push_back(couple);
                return other_team;
            }
        }
    }
    return other_team;
}

