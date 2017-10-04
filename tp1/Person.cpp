//
// Created by tomas on 29/09/17.
//

#include <sstream>

#include "Person.h"

Person::Person() : _id(-1) {
}

Person::Person(int id) : _id(id) {
}

std::string Person::id() const {
    std::stringstream ss;
    ss << _id;
    return ss.str();
}

bool Person::valid() {
    return _id != -1;
}

bool Person::is(int id) const {
    return id == _id;
}

int Person::int_id() const {
    return _id;
}

