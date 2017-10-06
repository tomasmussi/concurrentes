#ifndef TP1_MATCHPROCESS_H
#define TP1_MATCHPROCESS_H

#include "../model/Team.h"
#include "../ipc/LockFile.h"
#include "../ipc/MemoriaCompartida.h"
#include <list>
#include <map>

#define MATCH_PROBABILITY 50

/**
 * Proceso encargado de recibir dos parejas de equipos
 * y realizar la simulacion del partido y devolver un resultado
 * con los puntos que gano cada uno de los integrantes y el partido como salio
 * */
class MatchProcess {
private:
    pid_t _father_id;
    float _probability;
    int _score_team1;
    int _score_team2;
    LockFile _lock_matches;
    MemoriaCompartida<int>* _shm_matches;

    void run_match();
    void set_scores(int& score_winner, int& score_loser);
    void signal_court_manager();
public:
    MatchProcess(pid_t parent_process_id);
    ~MatchProcess();
    int get_match_result();
    void dispatch_match();
    std::string prettyName();
    void finalize();
};

#endif //TP1_MATCHPROCESS_H
