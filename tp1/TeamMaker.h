//
// Created by tomas on 29/09/17.
//

#ifndef TP1_TEAMMAKER_H
#define TP1_TEAMMAKER_H

#include <string>
#include "WorkerProcess.h"
#include "FifoLectura.h"
#include "FifoEscritura.h"

class TeamMaker : public WorkerProcess {
private:
    FifoLectura _fifo_read;
    FifoEscritura _fifo_write;

    FifoEscritura _fifo_prueba; // TODO ELIMINAR!!!!
public:
    TeamMaker(const std::string& fifo_read, const std::string& fifo_write);
    virtual void initialize();
    virtual int do_work();
    virtual void finalize();
    virtual std::string prettyName();
};


#endif //TP1_TEAMMAKER_H
