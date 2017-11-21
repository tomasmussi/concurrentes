#include <cstring>
#include <iostream>
#include "Cliente.h"
#include "Mensajes.h"
#include "ipc/Cola.h"

void Cliente::ejecutar() {
    Cola<mensaje> cola(MSG_ARCHIVO, CHAR_CLIENTE_SERVIDOR);
    mensaje m;
    m.mtype = TIEMPO;
    m.id = 3;
    strcpy(m.texto, "BSAS");
    cola.escribir(m);
    mensaje rta;
    cola.leer(m.id, &rta);
    std::cout << rta.texto << std::endl;

    m.mtype = MONEDA;
    m.id = 4;
    strcpy(m.texto, "USD");
    cola.escribir(m);
    cola.leer(m.id, &rta);
    std::cout << rta.texto << std::endl;
}