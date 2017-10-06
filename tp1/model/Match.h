#ifndef TP1_MATCH_H
#define TP1_MATCH_H

#include "Team.h"

class Match {
private:
    Team _team1;
    Team _team2;
    int _score_team1;
    int _score_team2;

public:
    Match();
    Match(const Team& team1, const Team& team2);
    std::string to_string();
    void set_match_status(int exit_code);
    bool finished();
    Team team1() const;
    Team team2() const;
};

#endif //TP1_MATCH_H
