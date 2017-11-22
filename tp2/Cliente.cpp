#include <cstring>
#include <iostream>
#include "Cliente.h"
#include "Constantes.h"

Cliente::Cliente(int tipoCliente, char* request) : cola(MSG_ARCHIVO, CHAR_CLIENTE_SERVIDOR),
                                                   _tipoCliente(tipoCliente), _request(request) {}

void Cliente::ejecutar() {
    mensaje m;
    m.mtype = NUEVA_CONEXION;
    cola.escribir(m);
    int resultado = cola.leer(RESPUESTA_NUEVA_CONEXION, &m);
    if (resultado != -1) {
        // El request va a ser a donde me indica el handShake
        int idCliente = m.id;
        m.mtype = idCliente;
        m.id = _tipoCliente;
        strcpy(m.texto, _request);
        cola.escribir(m);

        // Espero la respuesta en idCliente + 1
        mensaje rta;
        resultado = cola.leer(idCliente + 1, &rta);
        if (resultado != -1) {
            std::cout << "Cliente " << idCliente << " pidio por " << _tipoCliente << " " << _request <<
                " y recibio '" << rta.texto << "'" << std::endl;
        }
    }
}