#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#include "Servidor.h"
#include "ipc/SignalHandler.h"
#include "Constantes.h"
#include "Servicio.h"
#include "ServicioMonedas.h"
#include "ServicioTiempo.h"

Servidor::Servidor() : colaClientes(MSG_ARCHIVO, CHAR_CLIENTE_SERVIDOR),
                       colaServicios(MSG_ARCHIVO, CHAR_SERVIDOR_SERVICIOS) {
    servicios.push_back(new ServicioTiempo(colaServicios));
    servicios.push_back(new ServicioMonedas(colaServicios));
}

void Servidor::dispatchWorkerConsulta(mensaje request) {
    pid_t pid = fork();
    if (pid == 0) {
        // Worker
        mensaje response;
        if (request.mtype == TIEMPO) {
            // Consulta tipo TIEMPO
            colaServicios.escribir(request);
            colaServicios.leer(TIEMPO + 2, &response);
            colaClientes.escribir(response);
        } else if (request.mtype == MONEDA) {
            colaServicios.escribir(request);
            colaServicios.leer(MONEDA + 2, &response);
            colaClientes.escribir(response);
        } else {
            if (DEBUG) {
                std::cerr << "Consulta tipo: " << request.mtype << " no reconocida" << std::endl;
            }
        }
        if (DEBUG) {
            std::cout << "Se escribio '" << response.texto << "'" << std::endl;
        }
        deleteServicios();
        _exit(0);
    }
}

void Servidor::deleteServicios() {
    for (int i = 0; i < servicios.size(); i++) {
        delete(servicios[i]);
    }
}

void Servidor::dispatchServicios() {
    for (int i = 0; i < servicios.size(); i++) {
        Servicio* servicio = servicios[i];
        pid_t pid = fork();
        if (pid == 0) {
            servicio->ejecutar();
            deleteServicios();
            _exit(0);
        }
        procesosDespachados.push_back(pid);
        if (DEBUG) {
            std::cout << "Despachado servicio [" << pid << "]" << std::endl;
        }
    }
}

void Servidor::ejecutar() {
    clientesProcesados = 0;
    dispatchServicios();

    // El dispatch de servicios debe ocurrir antes de registrar handler
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

    if (DEBUG) {
        std::cout << "Servidor iniciado con cola" << std::endl;
    }

    while (sigint_handler.getGracefulQuit() == 0) {
        mensaje m;
        // De acuerdo al protocolo establecido, leo solo mensajes de tipo 1 y 2
        // y respondo a los clientes con id > 2
        int lectura = colaClientes.leer(-2, &m);
        if (lectura == -1) {
            if (errno != EINTR) {
                std::cerr << std::strerror(errno) << std::endl;
            }
        } else {
            if (DEBUG) {
                std::cout << "Recibida consulta cliente: " << m.id << " tipo: " << m.mtype << " por " << m.texto << std::endl;
            }
            // Despacho en un worker el procesamiento y respuesta para seguir procesando requests
            dispatchWorkerConsulta(m);
        }
    }

    for (std::list<pid_t>::iterator it = procesosDespachados.begin(); it != procesosDespachados.end(); it++) {
        // Enviar senial de finalizacion a servicios
        if (DEBUG) {
            std::cout << "Senializando [" << (*it) << "] de finalizacion de servidor" << std::endl;
        }
        kill((*it), SIGINT);
    }

    for (int i = 0; i < clientesProcesados; i++) {
        pid_t pid = wait(NULL);
        if (DEBUG) {
            std::cout << "Colectando worker despachado [" << pid << "]" << std::endl;
        }
    }

    for (int i = 0; i < servicios.size(); i++) {
        pid_t pid = wait(NULL);
        if (DEBUG) {
            std::cout << "Colectando worker despachado [" << pid << "]" << std::endl;
        }
    }

    deleteServicios();
    SignalHandler::destruir();
    colaClientes.destruir();
    colaServicios.destruir();
}