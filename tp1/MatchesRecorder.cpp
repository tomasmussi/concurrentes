//
// Created by tomas on 13/09/17.
//

#include <iostream>
#include "MatchesRecorder.h"

MatchesRecorder::MatchesRecorder() : _matches() {
}

void MatchesRecorder::add_match(const Match& match) {
    _matches.push_back(MatchPlayed(match));
}

void MatchesRecorder::list_all_matches() {
    for (std::list<MatchPlayed>::iterator it = _matches.begin(); it != _matches.end(); ++it) {
        std::cout << (*it).to_string() << std::endl;
    }
}
