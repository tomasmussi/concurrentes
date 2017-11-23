#ifndef TP2_CLIENTE_H
#define TP2_CLIENTE_H

#include "ipc/Cola.h"
#include "Mensajes.h"

class Cliente {
private:
    Cola<mensajeCS> cola;
    int _tipoCliente;
    char* _request;
public:
    void ejecutar();

    Cliente(int tipoCliente, char* request);
};

#endif //TP2_CLIENTE_H
