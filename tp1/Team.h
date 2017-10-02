//
// Created by tomas on 01/10/17.
//

#ifndef TP1_TEAM_H
#define TP1_TEAM_H

#include "Person.h"

class Team {
private:
    Person _p1;
    Person _p2;
public:
    Team();
    Team(const Person& p1, const Person& p2);
    std::string to_string();
    bool valid();
};


#endif //TP1_TEAM_H
