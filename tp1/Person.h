//
// Created by tomas on 29/09/17.
//

#ifndef TP1_PERSON_H
#define TP1_PERSON_H

#include <string>

class Person {
private:
    int _id;
public:
    Person();
    Person(int id);
    std::string id();
    bool valid();
    bool is(int id) const;
    int int_id() const;
};


#endif //TP1_PERSON_H
