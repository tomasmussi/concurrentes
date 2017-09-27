//
// Created by tomas on 26/09/17.
//

#ifndef TP1_PEOPLEREGISTERWORKER_H
#define TP1_PEOPLEREGISTERWORKER_H


#include "WorkerProcess.h"
#include "MemoriaCompartida.h"

/**
 * Clase encargada de registrar personas que quieren entrar al predio
 * de la playa para poder jugar al voley
 * */
class PeopleRegisterWorker : public WorkerProcess {
private:
    MemoriaCompartida<int> _shared_memory;
    int i;
public:
    PeopleRegisterWorker();
    virtual ~PeopleRegisterWorker();
    virtual int do_work();
};


#endif //TP1_PEOPLEREGISTERWORKER_H
