//
// Created by tomas on 11/09/17.
//

#ifndef TP1_MATCH_H
#define TP1_MATCH_H

#include "Team.h"
#include "LockFile.h"
#include "MemoriaCompartida.h"
#include <list>
#include <map>

#define MATCH_PROBABILITY 50

/**
 * Proceso encargado de recibir dos parejas de equipos
 * y realizar la simulacion del partido y devolver un resultado
 * con los puntos que gano cada uno de los integrantes y el partido como salio
 * */
class Match {
private:
    Team _team1;
    Team _team2;
    float _probability;
    int _score_team1;
    int _score_team2;
    pid_t _court_manager_pid;
    LockFile _lock_matches;
    MemoriaCompartida<int> _shm_matches;
    bool _son_process;

    void run_match();
    void set_scores(int& score_winner, int& score_loser);
    void signal_court_manager();
    int get_match_result();
public:
    Match();
    ~Match();
    Match(Team team1, Team team2, pid_t pid);
    pid_t dispatch_match();
    std::string to_string();
    Match(const Match& other_match);
    Match operator=(const Match& other_match);
    void set_match_status(int exit_code);
    bool finished();
    Team get_team1() const;
    Team get_team2() const;
};


#endif //TP1_MATCH_H
