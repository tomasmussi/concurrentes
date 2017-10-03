
#ifndef TP1_NEWPLAYERHANDLER_H
#define TP1_NEWPLAYERHANDLER_H

#include <unistd.h>
#include "EventHandler.h"
#include "FifoEscritura.h"

/**
 * Clase encargada de registrar personas que quieren entrar al predio
 * de la playa para poder jugar al voley. Por cada señal de SIGUSR1 que
 * se dispará este handler, que escribirá en un fifo para el proceso siguiente
 * */

class NewPlayerHandler : public EventHandler {
private:
    FifoEscritura _pipe_writer;
    int _i;
public:
    NewPlayerHandler(const std::string& fifo_write);
    ~NewPlayerHandler ();
    virtual int handleSignal ( int signum );
    virtual void initialize();
    virtual std::string prettyName();
};

#endif //TP1_NEWPLAYERHANDLER_H
