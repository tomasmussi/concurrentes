//
// Created by tomas on 13/09/17.
//

#include <sstream>
#include "MatchPlayed.h"

MatchPlayed::MatchPlayed(const Match &match) : _team1(), _team2(), _team1_score(0),
    _team2_score(0){
    _team1 = match.get_team1();
    _team2 = match.get_team2();
    _team1_score = match.get_team1_score();
    _team2_score = match.get_team2_score();
}

std::string MatchPlayed::to_string() {
    std::ostringstream stream;
    stream << _team1.front() << "-" << _team1.back() << " vs " << _team2.front() << "-" << _team2.back();
    stream << ": " << _team1_score << " a " << _team2_score;
    return stream.str();
}


