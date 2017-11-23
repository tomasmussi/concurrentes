#ifndef TP2_CLIENTE_H
#define TP2_CLIENTE_H

#include "ipc/Cola.h"
#include "Mensajes.h"

class Cliente {
private:
    Cola<mensaje> cola;
    int _tipoCliente;
    char* _request;
    char** _argv;
    mensaje _m;
    bool mensajeTipo;
    double temperatura;
    int presion;
    double humedad;
    double cambio;
public:
    void ejecutar();

    Cliente(int tipoCliente, char* request);
    Cliente(int tipoCliente, char** argv);
};

#endif //TP2_CLIENTE_H
