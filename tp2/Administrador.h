#ifndef TP2_ADMINISTRADOR_H
#define TP2_ADMINISTRADOR_H

#include "ipc/Cola.h"
#include "Mensajes.h"

class Administrador {
private:
    Cola<mensajeCS> cola;
    int parse_int(std::string s);
public:
    void ejecutar();

    Administrador();
};

#endif //TP2_ADMINISTRADOR_H
