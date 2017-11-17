//
// Created by tomas on 17/11/17.
//

#include <csignal>
#include "ipc/Cola.h"
#include "ipc/SignalHandler.h"
#include "Mensajes.h"
#include "Servicio.h"


void Servicio::ejecutar(const Cola<mensaje>& cola) {
    SignalHandler::getInstance()->registrarHandler( SIGINT, &sigint_handler);

    while ( sigint_handler.getGracefulQuit() == 0 ) {

    }
    // Escribir datos a disco

}

