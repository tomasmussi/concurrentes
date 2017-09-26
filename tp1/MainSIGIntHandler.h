//
// Created by tomas on 26/09/17.
//

#ifndef TP1_MAINSIGINTHANDLER_H
#define TP1_MAINSIGINTHANDLER_H

#include <unistd.h>
#include <list>
#include "EventHandler.h"

class MainSIGIntHandler : public EventHandler {
private:
    std::list<pid_t> _pid_list;
public:
    MainSIGIntHandler();
    ~MainSIGIntHandler ();
    virtual int handleSignal ( int signum );
    void add_pid_notification(pid_t pid);
};


#endif //TP1_MAINSIGINTHANDLER_H
