#include <cstring>
#include <iostream>
#include "Cliente.h"

Cliente::Cliente(int tipoCliente, char* request) : cola(MSG_ARCHIVO, CHAR_CLIENTE_SERVIDOR),
                                                   _tipoCliente(tipoCliente), _request(request) {}

void Cliente::ejecutar() {
    mensaje m;
    m.mtype = _tipoCliente;
    m.id = _tipoCliente + 2;
    strcpy(m.texto, _request);
    cola.escribir(m);
    mensaje rta;
    cola.leer(m.id, &rta);
    std::cout << rta.texto << std::endl;
}