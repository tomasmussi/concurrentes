//
// Created by tomas on 26/09/17.
//

#ifndef TP1_BEACHMANAGERWORKER_H
#define TP1_BEACHMANAGERWORKER_H

#include "WorkerProcess.h"
#include "MemoriaCompartida.h"
#include "FifoEscritura.h"
#include "FifoLectura.h"

/**
 * Clase encargada de administrar el predio de la playa, que es donde
 * las personas esperan para hacer parejas, ir a jugar a la cancha y
 * retirarse en caso de que ya hayan jugado K partidos o ya no puedan
 * armar equipo con nadie
 * */
class BeachManagerWorker : public WorkerProcess {
private:
    MemoriaCompartida<int> _shared_memory;
    FifoEscritura _pipe_writer;
    FifoLectura _pipe_reader;
    int _i;
public:
    BeachManagerWorker(const std::string& fifo_read, const std::string& fifo_write);

    virtual ~BeachManagerWorker();
    virtual void initialize();
    virtual void finalize();
    virtual int do_work();
    virtual std::string prettyName();
};


#endif //TP1_BEACHMANAGERWORKER_H
