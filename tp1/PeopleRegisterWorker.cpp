//
// Created by tomas on 26/09/17.
//

#include <iostream>
#include <unistd.h>

#include "PeopleRegisterWorker.h"

PeopleRegisterWorker::~PeopleRegisterWorker() {
}

int PeopleRegisterWorker::do_work() {
    sleep(5);
    std::cout << "PeopleRegisterWorker work done" << std::endl;
    return 5;
}
