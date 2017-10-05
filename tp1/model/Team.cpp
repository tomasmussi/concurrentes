//
// Created by tomas on 01/10/17.
//

#include "Team.h"

Team::Team() : _p1(), _p2() {
}

Team::Team(const Person &p1, const Person &p2) : _p1(p1), _p2(p2) {
}

std::string Team::to_string() const {
    return "(" + _p1.id() + ", " + _p2.id() + ")";
}

bool Team::valid() {
    return _p1.valid() && _p2.valid();
}

Person Team::get_person1() const {
    return _p1;
}

Person Team::get_person2() const {
    return _p2;
}