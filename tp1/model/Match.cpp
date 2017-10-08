#include <sstream>
#include "Match.h"

Match::Match(const Team& team1, const Team& team2) : _team1(team1), _team2(team2) {
}

Match::Match() : _team1(), _team2() {
}

std::string Match::to_string() {
    std::ostringstream stream;
    stream << _team1.to_string() << " vs " << _team2.to_string();
    stream << ": " << _score_team1 << " a " << _score_team2;
    return stream.str();
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

Team Match::team1() const {
    return _team1;
}

Team Match::team2() const {
    return _team2;
}

int Match::scoreTeam1() const {
    return _score_team1;
}

int Match::scoreTeam2() const {
    return _score_team2;
}

bool Match::valid() {
    return _team1.valid() && _team2.valid();
}
