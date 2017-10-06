#ifndef TP1_RESULTSREPORTER_H
#define TP1_RESULTSREPORTER_H

#include <string>
#include "WorkerProcess.h"
#include "../ipc/FifoLectura.h"
#include "../handlers/EventHandler.h"
#include "../model/Match.h"

/**
 * Clase encargada de informar los resultados de los partidos que
 * se van jugando a lo largo del torneo.
 * */
class ResultsReporter : public WorkerProcess {
private:
    FifoLectura _fifo_read;
public:
    ResultsReporter(const std::string& fifo_read);
    virtual ~ResultsReporter();
    virtual void initialize();
    virtual void finalize();
    virtual int do_work();
    virtual std::string prettyName();
};


#endif //TP1_RESULTSREPORTER_H
