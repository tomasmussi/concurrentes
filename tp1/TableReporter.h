#ifndef TP1_TABLEREPORTER_H
#define TP1_TABLEREPORTER_H

#include "WorkerProcess.h"
#include "MemoriaCompartida.h"

/**
 * Clase encargada de publicar periódicamente la tabla de posiciones
 * del torneo
 * */
class TableReporter : public WorkerProcess {
private:
    MemoriaCompartida<int> _shared_memory;
    int _time;
public:
    TableReporter();

    virtual ~TableReporter();
    virtual void initialize();
    virtual void finalize();
    virtual int do_work();
    virtual std::string prettyName();
};


#endif //TP1_TABLEREPORTER_H
