#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#include "Servidor.h"
#include "ipc/SignalHandler.h"

#define N_SERVICIOS 2

void Servidor::dispatchWorkerConsulta(const Cola<mensaje>& cola, mensaje request) {
    pid_t pid = fork();
    if (pid == 0) {
        // Worker
        if (request.mtype == TIEMPO) {
            // Consulta tipo TIEMPO
            mensaje response;
            response.mtype = request.id;
            response.id = -1;
            strcpy(response.texto, "El tiempo en BSAS es de 24°C, 1014 HectoPascales y humedad del 75%");
            cola.escribir(response);
        } else if (request.mtype == MONEDA) {
            mensaje response;
            response.mtype = request.id;
            response.id = -1;
            strcpy(response.texto, "La moneda USD esta a 1/18 respecto a ARS");
            cola.escribir(response);
        } else {
            if (DEBUG) {
                std::cerr << "Consulta tipo: " << request.mtype << " no reconocida" << std::endl;
            }
        }
        _exit(0);
    }
}

void Servidor::dispatchServicios(const Cola<mensaje>& servicio) {
    for (int i = 0; i < N_SERVICIOS; i++) {
        pid_t pid = fork();
        // TODO: Despachar servicios posta
        if (pid == 0) {
            if (i == 0) {
                std::cout << "Servicio de tiempo" << std::endl;
            } else if (i == 1) {
                std::cout << "Servicio de moneda" << std::endl;
            }
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
    Cola<mensaje> colaClientes(MSG_ARCHIVO, CHAR_CLIENTE_SERVIDOR);
    Cola<mensaje> colaServicios(MSG_ARCHIVO, CHAR_SERVIDOR_SERVICIOS);
    dispatchServicios(colaServicios);

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
            dispatchWorkerConsulta(colaClientes, m);
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

    for (int i = 0; i < N_SERVICIOS; i++) {
        pid_t pid = wait(NULL);
        if (DEBUG) {
            std::cout << "Colectando worker despachado [" << pid << "]" << std::endl;
        }
    }

    SignalHandler::destruir();
    colaClientes.destruir();
    colaServicios.destruir();
}