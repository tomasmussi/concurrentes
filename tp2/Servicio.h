//
// Created by tomas on 17/11/17.
//

#ifndef TP2_SERVICIO_H
#define TP2_SERVICIO_H

#include "ipc/SIGINT_Handler.h"
#include "ipc/Cola.h"
#include "Mensajes.h"

class Servicio {
private:
    SIGINT_Handler sigint_handler;
public:
    void ejecutar(const Cola<mensaje>& cola);
};


#endif //TP2_SERVICIO_H
