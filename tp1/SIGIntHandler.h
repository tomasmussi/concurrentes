//
// Created by tomas on 26/09/17.
//

#ifndef TP1_SIGINTHANDLER_H
#define TP1_SIGINTHANDLER_H

#include <signal.h>

#include "EventHandler.h"

class SIGIntHandler : public EventHandler {
private:
    sig_atomic_t gracefulQuit;
public:
    SIGIntHandler();
    ~SIGIntHandler ();
    virtual int handleSignal ( int signum );
    sig_atomic_t getGracefulQuit () const;
};


#endif //TP1_SIGINTHANDLER_H
