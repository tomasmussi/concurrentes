//
// Created by tomas on 11/09/17.
//

#ifndef TP1_SUBSCRIBER_H
#define TP1_SUBSCRIBER_H
#define MIN_SUBSCRIBERS 10

#include <cstddef>
#include <list>
#include "TournamentManager.h"

/**
 * Proceso encargado de tomar nuevos participantes en la playa y
 * derivarlos al TournamentManager para poder jugar en el torneo
 * */

class Subscriber {
private:
    std::list<int> _people_subscribed;
    TournamentManager* _manager;
public:
    Subscriber();
    void subscribe_person(int number);

    void add_manager(TournamentManager* manager);

    void do_job();
};


#endif //TP1_SUBSCRIBER_H
