#include <csignal>
#include "ipc/Cola.h"
#include "ipc/SignalHandler.h"
#include "Mensajes.h"
#include "Servicio.h"
#include <iostream>
#include <sstream>

Servicio::Servicio(const Cola<mensaje>& cola, int tipo) : _cola(cola), _tipoServicio(tipo) {}

double Servicio::parseDouble(std::string& string) {
    std::istringstream ss(string);
    double x;
    std::stringstream s;
    if (!(ss >> x)) {
        return -1;
    }
    return x;
}

void Servicio::ejecutar() {
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

    while (sigint_handler.getGracefulQuit() == 0) {
        // En ningun lado dice que el servicio tiene que ser concurrente, solo pide que el "portal" lo sea
        mensaje msg;
        if (DEBUG) {
            std::cout << "Servicio " << _tipoServicio << " esperando para leer" << std::endl;
        }
        int resultado = _cola.leer(_tipoServicio, &msg);
        // Esto es para los casos en que se interrumpio la lectura, por lo que no se tiene que escribir
        if (resultado != -1) {
            if (DEBUG) {
                std::cout << "Servicio " << _tipoServicio << " leyo" << std::endl;
            }
            if (msg.write) {
                // Escritura nuevo valor
                actualizarDato(msg);
            }
            // Lectura de valor
            strcpy(msg.texto, getDato(std::string(msg.texto)).c_str());
            // En id me indicaron en donde tenia que responder
            msg.mtype = msg.id;
            _cola.escribir(msg);
            if (DEBUG) {
                std::cout << "Servicio " << _tipoServicio << " escribio para el cliente " << msg.id << std::endl;
            }
        }
    }
    dumpData();
}

