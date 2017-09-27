//
// Created by tomas on 26/09/17.
//

#include "PeopleRegisterWorker.h"
#include <unistd.h>

PeopleRegisterWorker::PeopleRegisterWorker() : _shared_memory("/bin/bash", 'a'), i(0) {
}

PeopleRegisterWorker::~PeopleRegisterWorker() {
}

int PeopleRegisterWorker::do_work() {
    _shared_memory.escribir(i++);
    sleep(3);
    return 5;
}
