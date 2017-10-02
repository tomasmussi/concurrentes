#ifndef TP1_RESULTSREPORTER_H
#define TP1_RESULTSREPORTER_H

#include "WorkerProcess.h"
#include "MemoriaCompartida.h"

/**
 * Clase encargada de informar los resultados de los partidos que
 * se van jugando a lo largo del torneo.
 * */
class ResultsReporter : public WorkerProcess {
private:
    MemoriaCompartida<int> _shared_memory;
public:
    ResultsReporter();

    virtual ~ResultsReporter();
    virtual void initialize();
    virtual void finalize();
    virtual int do_work();
    virtual std::string prettyName();
};


#endif //TP1_RESULTSREPORTER_H
