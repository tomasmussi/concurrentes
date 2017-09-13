//
// Created by tomas on 13/09/17.
//

#ifndef TP1_MATCHESRECORDER_H
#define TP1_MATCHESRECORDER_H

#include "Match.h"
#include "MatchPlayed.h"

/**
 * Proceso encargado de mantener registro de todos los partidos jugados
 * Mantiene las parejas que jugaron y el resultado que se obtuvo
 * */
class MatchesRecorder {
private:
    std::list<MatchPlayed> _matches;
public:
    MatchesRecorder();
    void add_match(const Match& match);
    void list_all_matches();
};


#endif //TP1_MATCHESRECORDER_H
