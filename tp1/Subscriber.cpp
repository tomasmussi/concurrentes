//
// Created by tomas on 11/09/17.
//

#include "Subscriber.h"

Subscriber::Subscriber() : _people_subscribed(), _manager(NULL) {
}

void Subscriber::subscribe_person(int number) {
    _people_subscribed.push_back(number);
    if (_people_subscribed.size() == MIN_SUBSCRIBERS) {
        _manager->start_competition(_people_subscribed);
    } else if (_manager->started()) {
        _manager->add_competitor(number);
    }
}

void Subscriber::add_manager(TournamentManager* tournament_manager) {
    _manager = tournament_manager;
}

void Subscriber::do_job() {

}
