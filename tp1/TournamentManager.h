//
// Created by tomas on 11/09/17.
//

#ifndef TP1_TOURNAMENTMANAGER_H
#define TP1_TOURNAMENTMANAGER_H


#include <list>
#include <map>
#include "MatchesRecorder.h"

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

    // ID persona que pasa el Subscriber
    std::list<int> _people;

    // Registro de personas que hicieron pareja con otras personas
    // Se accede _pairs[pers1][pers2], si existe la clave es porque hicieron pareja
    std::map<int, std::map<int,int > > _pairs;

    // Canchas del torneo (todavia no lo use)
    std::map<int, std::map<int,int > > _tournament_courts;

    // Mapa que tiene ID persona y puntos individuales que acumulo
    std::map<int, int> _people_individual_positions;

    MatchesRecorder* _matches_recorder;

    std::list<int> get_team();
    std::list<int> get_team_for(const std::list<int> & team);
    bool in_team(const std::list<int>& team, int person);
    bool no_more_couples();

public:
    TournamentManager(int k, int rows, int columns);
    ~TournamentManager();
    void start_competition(const std::list<int> & subscribed_people);
    void add_competitor(const int person);
    bool started();
    bool do_job();

    // Esto es dudoso, sacar
    MatchesRecorder* get_recorder();
};


#endif //TP1_TOURNAMENTMANAGER_H
