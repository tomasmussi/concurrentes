#ifndef TP1_SIGNALHANDLER_H
#define TP1_SIGNALHANDLER_H

#include "../handlers/EventHandler.h"
#include <signal.h>

class SignalHandler {
private:
    static SignalHandler* instance;
    static EventHandler* signal_handlers [ NSIG ];

    SignalHandler(void);
    static void dispatcher(int signum);

public:
    static SignalHandler* getInstance();
    static void destroy();
    EventHandler* registrarHandler(int signum, EventHandler* eh);
    int removerHandler(int signum);
};

#endif //TP1_SIGNALHANDLER_H
