//
// Created by tomas on 11/09/17.
//

#ifndef TP1_MATCH_H
#define TP1_MATCH_H


#include <list>

class Match {
private:
    std::list<int> _team1;
    std::list<int> _team2;
    int _row;
    int _column;
public:
    Match(const std::list<int>& team1, const std::list<int>& team2, int row, int column);
};


#endif //TP1_MATCH_H
