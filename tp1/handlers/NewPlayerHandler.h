#ifndef TP1_NEWPLAYERHANDLER_H
#define TP1_NEWPLAYERHANDLER_H

#include <unistd.h>
#include "EventHandler.h"
#include "../ipc/FifoEscritura.h"
#include "../ipc/LockFile.h"
#include "../ipc/MemoriaCompartida.h"

#define MAX_GONE_PLAYERS 128

/**
 * Clase encargada de registrar personas que quieren entrar al predio
 * de la playa para poder jugar al voley. Por cada señal de SIGUSR1 que
 * se dispará este handler, que escribirá en un fifo para el proceso siguiente
 * */

class NewPlayerHandler : public EventHandler {
private:
    FifoEscritura _pipe_writer;
    LockFile _lock_shm_gone_players;
    MemoriaCompartida<int>* _shm_gone_players;
    int _i;

    void initialize_shm_gone_players();
    void destroy_shm_gone_players();
    int read_shm_gone_players();
    int _last_gone;
public:
    NewPlayerHandler(const std::string& fifo_write);
    ~NewPlayerHandler();
    virtual int handleSignal(int signum);
    virtual void initialize();
    virtual std::string prettyName();
};

#endif //TP1_NEWPLAYERHANDLER_H
