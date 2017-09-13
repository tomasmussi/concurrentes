//
// Created by tomas on 13/09/17.
//

#ifndef TP1_MATCHPLAYED_H
#define TP1_MATCHPLAYED_H

#include <string>
#include "Match.h"

/**
 * Clase que mantiene registro de un partido jugado por persona
 * */
class MatchPlayed {
private:
    std::list<int> _team1;
    std::list<int> _team2;
    int _team1_score;
    int _team2_score;
public:
    MatchPlayed(const Match& match);
    std::string to_string();
};


#endif //TP1_MATCHPLAYED_H
