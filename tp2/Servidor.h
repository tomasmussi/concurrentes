//
// Created by tomas on 17/11/17.
//

#ifndef TP2_SERVIDOR_H
#define TP2_SERVIDOR_H


#include "ipc/Cola.h"
#include "Mensajes.h"
#include "ipc/SIGINT_Handler.h"

class Servidor {
private:
    SIGINT_Handler sigint_handler;
    int clientesProcesados;
    void dispatchWorkerConsulta(const Cola<mensaje>& cola, mensaje request);

public:
    void ejecutar();


};


#endif //TP2_SERVIDOR_H
