#ifndef TP1_MATCHPROCESS_H
#define TP1_MATCHPROCESS_H

#include "../model/Team.h"
#include "../handlers/EventHandler.h"
#include "../constants.h"
#include <list>
#include <map>
#include "../utils/Logger.h"

/**
 * Proceso encargado de recibir dos parejas de equipos
 * y realizar la simulacion del partido y devolver un resultado
 * con los puntos que gano cada uno de los integrantes y el partido como salio
 * */
class MatchProcess: public EventHandler {
private:
    pid_t _father_id;
    float _probability;
    int _score_team1;
    int _score_team2;

    void set_scores(int& score_winner, int& score_loser);
    void signal_court_manager();
public:
    MatchProcess(pid_t parent_process_id);
    ~MatchProcess();
    int get_match_result();
    void run_match();
    std::string prettyName();
    void finalize();
    int handleSignal(int signum);
};

#endif //TP1_MATCHPROCESS_H
