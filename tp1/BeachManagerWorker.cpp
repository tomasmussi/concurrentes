//
// Created by tomas on 26/09/17.
//

#include "BeachManagerWorker.h"
#include <iostream>
#include <unistd.h>

BeachManagerWorker::BeachManagerWorker(Pipe pipe) : _shared_memory("/bin/bash", 'a'),
    _pipe(pipe) {
    _pipe.set_mode(Pipe::MODE_READ);
}

BeachManagerWorker::~BeachManagerWorker() {
}

int BeachManagerWorker::do_work() {
    /*int i = _shared_memory.leer();
    std::cout << "leyendo: " << i << std::endl;*/
    sleep(1);
    int j;
    _pipe.read_pipe(&j, sizeof(int));
    std::cout << "lei del pipe: " << j << " cuando llegue a 5 se va a quedar bloqueado" << std::endl;
    return 3;
}