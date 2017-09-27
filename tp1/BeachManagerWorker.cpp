//
// Created by tomas on 26/09/17.
//

#include "BeachManagerWorker.h"
#include <iostream>
#include <unistd.h>

BeachManagerWorker::BeachManagerWorker(Pipe pipe) : _shared_memory("/bin/bash", 'a'),
    _pipe(pipe), _pipe_writer("/tmp/archivo_fifo") {
    std::cout << "beach worker" << std::endl;
    // _pipe.set_mode(Pipe::MODE_READ);
    _pipe_writer.abrir();
}

BeachManagerWorker::~BeachManagerWorker() {
}

int BeachManagerWorker::do_work() {
    sleep(1);
    int i = 8;
    _pipe_writer.escribir(&i, sizeof(int));
    std::cout << "escribi fifo: " << i << std::endl;
    return 3;
}