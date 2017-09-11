//
// Created by tomas on 11/09/17.
//

#ifndef TP1_TOURNAMENTMANAGER_H
#define TP1_TOURNAMENTMANAGER_H


#include <list>
#include <map>

/**
 * Proceso encargado de armar parejas que no se repitan
 * y derivarlas en procesos encargados de jugar los partidos
 * */
class TournamentManager {
private:
    int _k;
    int _rows;
    int _columns;
    bool _started;
    std::list<int> _people;
    std::map<int, std::map<int,int > > _pairs;
    std::map<int, std::map<int,int > > _tournament_courts;

    std::list<int> get_team();
    std::list<int> get_team_for(const std::list<int> & team);
    bool in_team(const std::list<int>& team, int person);

public:
    TournamentManager(int k, int rows, int columns);
    void start_competition(const std::list<int> & subscribed_people);
    void add_competitor(const int person);
    bool started();

    bool finished();

    void do_job();
};


#endif //TP1_TOURNAMENTMANAGER_H
