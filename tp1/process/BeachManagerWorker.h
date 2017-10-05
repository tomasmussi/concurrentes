#ifndef TP1_BEACHMANAGERWORKER_H
#define TP1_BEACHMANAGERWORKER_H

#include "WorkerProcess.h"
#include "../ipc/MemoriaCompartida.h"
#include "../ipc/FifoEscritura.h"
#include "../ipc/FifoLectura.h"

/**
 * Clase encargada de administrar el predio de la playa, que es donde
 * las personas esperan para hacer parejas, ir a jugar a la cancha y
 * retirarse en caso de que ya hayan jugado K partidos o ya no puedan
 * armar equipo con nadie
 * */
class BeachManagerWorker : public WorkerProcess {
private:
    FifoEscritura _pipe_writer;
    FifoLectura _pipe_reader;
    int _i;
    int _m;

    void sendPerson(int i);
public:
    BeachManagerWorker(int m, const std::string& fifo_read, const std::string& fifo_write);
    virtual ~BeachManagerWorker();
    virtual void initialize();
    virtual void finalize();
    virtual int do_work();
    virtual std::string prettyName();
};

#endif //TP1_BEACHMANAGERWORKER_H
