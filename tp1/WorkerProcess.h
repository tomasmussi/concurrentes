//
// Created by tomas on 26/09/17.
//

#ifndef TP1_WORKERPROCESS_H
#define TP1_WORKERPROCESS_H

/**
 * Clase que encapsula el trabajo de cada uno de los procesos que componen el programa
 * Todos los procesos luego de ejecutarse entran en un loop y cada uno de ellos debe definir
 * cual es su tarea interna
 * */
class WorkerProcess {
public:
    virtual ~WorkerProcess();
    virtual int do_work() = 0;
    int loop();
};


#endif //TP1_WORKERPROCESS_H
