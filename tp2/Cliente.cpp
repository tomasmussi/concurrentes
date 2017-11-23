#include <cstring>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include "Cliente.h"
#include "Constantes.h"

Cliente::Cliente(int tipoCliente, char* request) : cola(MSG_ARCHIVO, CHAR_CLIENTE_SERVIDOR),
                                                   _tipoCliente(tipoCliente), _request(request) {
    // Cliente comun
    mensajeTipo = false;
}

Cliente::Cliente(int tipoCliente, char** argv) : cola(MSG_ARCHIVO, CHAR_CLIENTE_SERVIDOR),
                                                   _tipoCliente(tipoCliente), _request(argv[3]) {
    // Administrador
    mensajeTipo = true;
    if (_tipoCliente == TIEMPO) {
        temperatura = atof(argv[4]);
        presion = atoi(argv[5]);
        humedad = atof(argv[6]);
    } else if (_tipoCliente == MONEDA) {
        cambio = atof(argv[4]);
    }
}

void Cliente::ejecutar() {

    _m.mtype = NUEVA_CONEXION;
    cola.escribir(_m);
    int resultado = cola.leer(RESPUESTA_NUEVA_CONEXION, &_m);
    if (resultado != -1) {
        // El request va a ser a donde me indica el handShake
        int idCliente = _m.id;
        _m.mtype = idCliente;
        _m.id = _tipoCliente;
        _m.write = mensajeTipo;
        strcpy(_m.texto, _request);
        if (_tipoCliente == TIEMPO) {
            _m.tiemp.temperatura = temperatura;
            _m.tiemp.presion = presion;
            _m.tiemp.humedad = humedad;
        } else if (_tipoCliente == MONEDA) {
            _m.cambio = cambio;
        }
        cola.escribir(_m);

        // Espero la respuesta en idCliente + 1
        mensaje rta;
        resultado = cola.leer(idCliente + 1, &rta);
        if (resultado != -1) {
            std::cout << "Cliente " << idCliente << " pidio por " << _tipoCliente << " " << _request <<
                " y recibio '" << rta.texto << "'" << std::endl;
        }
    }

}