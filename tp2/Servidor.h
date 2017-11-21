#ifndef TP2_SERVIDOR_H
#define TP2_SERVIDOR_H

#include <list>
#include "ipc/Cola.h"
#include "Mensajes.h"
#include "ipc/SIGINT_Handler.h"

class Servidor {
private:
    SIGINT_Handler sigint_handler;
    int clientesProcesados;
    std::list<pid_t> procesosDespachados;

    Cola<mensaje> colaServicios;
    Cola<mensaje> colaClientes;

    void dispatchServicios();
    void dispatchWorkerConsulta(mensaje request);
public:
    Servidor();
    void ejecutar();
};


#endif //TP2_SERVIDOR_H
