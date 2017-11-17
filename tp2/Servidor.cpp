//
// Created by tomas on 17/11/17.
//

#include <iostream>
#include <unistd.h>

#include "Servidor.h"
#include "ipc/SignalHandler.h"


void Servidor::dispatchWorkerConsulta(const Cola<mensaje>& cola, mensaje request) {
    pid_t pid = fork();
    if (pid == 0) {
        // Worker
        if (request.mtype == 1) {
            // Consulta tipo TIEMPO
            mensaje response;
            response.mtype = request.id;
            response.id = -1;
            strcpy(response.texto, "El tiempo en BSAS es de 24°C, 1014 HectoPascales y humedad del 75%");
            cola.escribir(response);
        } else if (request.mtype) {
            mensaje response;
            response.mtype = request.id;
            response.id = -1;
            strcpy(response.texto, "La moneda USD esta a 1/18 respecto a ARP");
            cola.escribir(response);
        } else {
            if (DEBUG) {
                std::cerr << "Consulta tipo: " << request.mtype << " no reconocida" << std::endl;
            }
        }
        _exit(0);
    }
}


void Servidor::ejecutar() {
    Cola<mensaje> cola(MSG_ARCHIVO, CHAR_CLIENTE_SERVIDOR);
    SignalHandler::getInstance()->registrarHandler( SIGINT, &sigint_handler);

    if (DEBUG) {
        std::cout << "Servidor iniciado con cola" << std::endl;
    }

    while ( sigint_handler.getGracefulQuit() == 0 ) {
        mensaje m;
        // De acuerdo al protocolo establecido, leo solo mensajes de tipo 1 y 2
        // y respondo a los clientes con id > 2
        cola.leer(-2, &m);
        // Despacho en un worker el procesamiento y respuesta para seguir procesando requests
        dispatchWorkerConsulta(cola, m);
    }

    SignalHandler :: destruir ();
    cola.destruir();
}