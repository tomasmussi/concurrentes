#ifndef TP1_RESULTSREPORTER_H
#define TP1_RESULTSREPORTER_H

#include <string>
#include <map>
#include "WorkerProcess.h"
#include "../ipc/FifoLectura.h"
#include "../handlers/EventHandler.h"
#include "../model/Match.h"
#include "../constants.h"
#include <utility>

/**
 * Clase encargada de informar los resultados de los partidos que
 * se van jugando a lo largo del torneo.
 * */
 struct player {
   int points;
   int matchesPlayed;
 };

typedef struct player player_t;

class ResultsReporter : public WorkerProcess {
private:
    FifoLectura _fifo_read;
    int _countdown;
    std::map<std::string, player_t> _pointsTable;
    void updateTableWithMatch(Match match);
    void updateTableWithTeam(Team team, int points);
    void updateTableWithPerson(Person person, int points);
    int pointsFromScore(int score, int opponent_score) const;
    void showPointsTable();
    void showChampion();

    static bool comp(std::pair<std::string,player_t> a, std::pair<std::string,player_t> b) { return a.second.points > b.second.points;}
public:
    ResultsReporter(const std::string& fifo_read);
    virtual ~ResultsReporter();
    virtual void initialize();
    virtual void finalize();
    virtual int do_work();
    virtual std::string prettyName();
};


#endif //TP1_RESULTSREPORTER_H
