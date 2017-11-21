#include <csignal>
#include "ipc/Cola.h"
#include "ipc/SignalHandler.h"
#include "Mensajes.h"
#include "Servicio.h"

void Servicio::ejecutar(const Cola<mensaje>& cola) {
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

    while ( sigint_handler.getGracefulQuit() == 0 ) {
        // En ningun lado dice que el servicio tiene que ser concurrente, solo pide que el "portal" lo sea
    }
    // TODO: Escribir datos a disco
}

