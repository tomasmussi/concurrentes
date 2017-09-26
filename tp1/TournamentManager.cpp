//
// Created by tomas on 11/09/17.
//
#include <iostream>
#include <algorithm>
#include "TournamentManager.h"
#include "Subscriber.h"


TournamentManager::TournamentManager(int k, int rows, int columns) :
    _k(k), _rows(rows), _columns(columns), _started(false), _people(), _tournament_courts(),
    _people_individual_positions(), _matches_recorder(NULL) {
    for (int row = 0; row < _rows; row++) {
        for (int col = 0; col < _columns; col++) {
            _tournament_courts[row][col] = 0;
        }
    }
    _matches_recorder = new MatchesRecorder();
}

TournamentManager::~TournamentManager() {
    delete _matches_recorder;
}

void TournamentManager::start_competition(const std::list<int> & subscribed_people) {
    if (subscribed_people.size() < MIN_SUBSCRIBERS) {
        return;
    }
    std::copy(subscribed_people.begin(), subscribed_people.end(),
             std::back_inserter(_people));
    std::list<int>::const_iterator it;
    for (it = subscribed_people.begin(); it != subscribed_people.end(); ++it) {
        _people_individual_positions[*it] = 0;
    }
    _started = true;
}

void TournamentManager::add_competitor(const int person) {
    _people.push_back(person);
    _people_individual_positions[person] = 0;
}

bool TournamentManager::started() {
    return _started;
}

/**
 * Se necesitan 4 o mas parejas que no hayan superado el numero K de parejas posibles
 * */
bool TournamentManager::no_more_couples() {
    int left_couples = 0;
    std::list<int>::iterator it;
    for (it = _people.begin(); it != _people.end(); ++it) {
        int person_couples = _pairs[*it].size();
        if (person_couples < _k) {
            left_couples++;
        }
    }
    return left_couples < 4;
}

/**
 * Hay dos motivos que provocan la finalizacion del torneo:
 * 1) Todas las personas ya jugaron K partidos
 * 2) Por mas que haya gente sin jugar K partidos, no se pueden formar mas equipos
 * porque ya todos hicieron pareja con todos
 * */
bool TournamentManager::do_job() {
    if (no_more_couples()) {
        return false;
    }
    std::list<int> team = get_team();
    if (team.size() == 0) {
        return false;
    }
    std::list<int> opponents = get_team_for(team);
    if (opponents.size() == 0) {
        return false;
    }

    Match match(team, opponents, 1, 1, 0.5);
    _matches_recorder->add_match(match);
    return true;
}

std::list<int> TournamentManager::get_team() {
    std::list<int> team;
    for (size_t person = 0; person < _people.size(); person++) {
        for (size_t couple = 0; couple < _people.size(); couple++) {
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
    for (size_t person = 0; person < _people.size(); person++) {
        if (in_team(team, person)) {
            continue;
        }
        for (size_t couple = 0; couple < _people.size(); couple++) {
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

MatchesRecorder* TournamentManager::get_recorder() {
    return _matches_recorder;
}


