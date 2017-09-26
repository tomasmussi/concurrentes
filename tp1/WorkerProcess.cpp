//
// Created by tomas on 26/09/17.
//

#include "WorkerProcess.h"

WorkerProcess::~WorkerProcess() {
}

int WorkerProcess::loop() {
    return do_work();
}
