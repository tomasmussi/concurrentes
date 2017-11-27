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

    clientesProcesados = 0;
    dispatchServicios();

    // El dispatch de servicios debe ocurrir antes de registrar handler
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

    if (DEBUG) {
        std::cout << "Servidor inicializado" << std::endl;
    }
}

mensajeSS Servidor::enviarYRecibir(mensajeCS request, int idCliente, int tipo) {
    // Le envío al servicio correspondiente (Indicado mediante el mtype) el id dónde se espera la respuesta
    mensajeSS req;
    req.mtype = tipo;
    req.id = idCliente;
    req.admin = request.admin;
    strcpy(req.texto, request.texto);
    colaServicios.escribir(req);

    // Espero la respuesta en donde le indiqué al servicio
    mensajeSS response;
    int lectura = colaServicios.leer(idCliente, &response);
    if (lectura != -1) {
        // Escribo exactamente la respuesta recibida del servicio, en la cola de clientes, al cliente correspondiente
        mensajeCS resp;
        resp.mtype = idCliente + 1;
        strcpy(resp.texto, response.texto);
        colaClientes.escribir(resp);
    }
    return response;
}

void Servidor::dispatchWorkerConsulta() {
    // Obtengo el numero de cliente antes del fork, de forma de que sólo el servidor modifique el atributo
    // clientesProcesados. El idCliente empieza en 3 de forma de no pisarse con NUEVA_CONEXION ni RESPUESTA_NUEVA_CONEXION
    int idCliente = (++clientesProcesados * 2) + 1;
    pid_t pid = fork();
    if (pid == 0) {
        // Worker
        mensajeCS handShakeRequest;
        handShakeRequest.mtype = RESPUESTA_NUEVA_CONEXION;
        handShakeRequest.id = idCliente;
        colaClientes.escribir(handShakeRequest);

        mensajeCS request;
        int lectura = colaClientes.leer(idCliente, &request);
        if (lectura != -1) {
            if (DEBUG) {
                std::map<int, std::string> tipoServicio;
                tipoServicio[TIEMPO] = "TIEMPO";
                tipoServicio[MONEDA] = "MONEDA";

                std::cout << "Worker [" << getpid() << "] - Recibida consulta cliente " << idCliente << ": " <<
                    tipoServicio[request.tipo] << " por " << request.texto << std::endl;
            }

            // Hago un request al servicio correspondiente
            mensajeSS response;
            if (request.tipo == TIEMPO) {
                response = enviarYRecibir(request, idCliente, TIEMPO);
            } else if (request.tipo == MONEDA) {
                response = enviarYRecibir(request, idCliente, MONEDA);
            } else {
                if (DEBUG) {
                    std::cerr << "Consulta tipo: " << request.mtype << " no reconocida" << std::endl;
                }
            }
            if (DEBUG) {
                std::cout << "Worker [" << getpid() << "] - Se escribio '" << response.texto << "'" << std::endl;
            }
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
            // Servicio
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
    while (sigint_handler.getGracefulQuit() == 0) {
        mensajeCS m;
        // De acuerdo al protocolo establecido, el servidor lee requests de tipo 1 y despacha un worker para que atienda
        // dicho request
        int lectura = colaClientes.leer(NUEVA_CONEXION, &m);
        if (lectura != -1) {
            if (DEBUG) {
                std::cout << "Servidor - Recibida una nueva conexion" << std::endl;
            }
            // Despacho en un worker el procesamiento y respuesta para seguir procesando requests
            dispatchWorkerConsulta();
        }
    }

    // Primero se espera a que terminen los workers con los servicios aún funcionando
    for (int i = 0; i < clientesProcesados; i++) {
        pid_t pid = wait(NULL);
        if (DEBUG) {
            std::cout << "Colectando worker de cliente despachado [" << pid << "]" << std::endl;
        }
    }

    // Y luego se avisa a los servicios que deben terminar
    for (std::list<pid_t>::iterator it = procesosDespachados.begin(); it != procesosDespachados.end(); it++) {
        if (DEBUG) {
            std::cout << "Senializando al servicio [" << (*it) << "] de finalizacion de servidor" << std::endl;
        }
        kill((*it), SIGINT);
    }

    for (int i = 0; i < servicios.size(); i++) {
        pid_t pid = wait(NULL);
        if (DEBUG) {
            std::cout << "Colectando servicio despachado [" << pid << "]" << std::endl;
        }
    }

    deleteServicios();
    SignalHandler::destruir();
    colaClientes.destruir();
    colaServicios.destruir();
}
