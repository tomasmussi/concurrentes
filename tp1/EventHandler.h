//
// Created by tomas on 26/09/17.
//

#ifndef TP1_EVENTHANDLER_H
#define TP1_EVENTHANDLER_H


class EventHandler {
public:
    virtual int handleSignal ( int signum ) = 0;
    virtual ~EventHandler () {};
};


#endif //TP1_EVENTHANDLER_H
