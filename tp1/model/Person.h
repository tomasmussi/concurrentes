#ifndef TP1_PERSON_H
#define TP1_PERSON_H

#include <string>

class Person {
private:
    int _id;

public:
    Person();
    Person(int id);
    std::string id() const;
    bool valid();
    bool is(int id) const;
    int int_id() const;
};

#endif //TP1_PERSON_H
