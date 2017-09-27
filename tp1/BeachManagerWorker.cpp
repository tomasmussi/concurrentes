//
// Created by tomas on 26/09/17.
//

#include "BeachManagerWorker.h"
#include <iostream>
#include <unistd.h>

BeachManagerWorker::BeachManagerWorker() : _shared_memory("/bin/bash", 'a') {
}

BeachManagerWorker::~BeachManagerWorker() {
}

int BeachManagerWorker::do_work() {
    int i = _shared_memory.leer();
    std::cout << "leyendo: " << i << std::endl;
    sleep(1);
    return 3;
}