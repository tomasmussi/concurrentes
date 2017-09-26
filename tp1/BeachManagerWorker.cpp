//
// Created by tomas on 26/09/17.
//

#include "BeachManagerWorker.h"

#include <iostream>
#include <unistd.h>

BeachManagerWorker::~BeachManagerWorker() {
}

int BeachManagerWorker::do_work() {
    sleep(3);
    std::cout << "BeachManagerWorker work done" << std::endl;
    return 3;
}