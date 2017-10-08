#ifndef TP1_WORKERPROCESS_H
#define TP1_WORKERPROCESS_H

#include <iostream>
#include <csignal>
#include "../handlers/SIGIntHandler.h"

/**
 * Clase que encapsula el trabajo de cada uno de los procesos que componen el programa
 * Todos los procesos luego de ejecutarse entran en un loop y cada uno de ellos debe definir
 * cual es su tarea interna
 * */
class WorkerProcess {
private:
    SIGIntHandler sigint_handler;
protected:
    sig_atomic_t graceQuit() const;
public:
    virtual ~WorkerProcess();
    virtual void initialize();
    virtual void finalize();
    virtual int do_work() = 0;
    virtual std::string prettyName() = 0;
    int loop();
};

#endif //TP1_WORKERPROCESS_H
