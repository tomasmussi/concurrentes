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
class ResultsReporter : public WorkerProcess {
private:
    FifoLectura _fifo_read;
    int _countdown;
    std::map<std::string, int> _pointsTable;
    void updateTableWithMatch(Match match);
    void updateTableWithTeam(Team team, int points);
    void updateTableWithPerson(Person person, int points);
    int pointsFromScore(int score, int opponent_score) const;
    void showPointsTable();

    static bool comp(std::pair<std::string,int> a, std::pair<std::string,int> b) { return a.second > b.second;}
public:
    ResultsReporter(const std::string& fifo_read);
    virtual ~ResultsReporter();
    virtual void initialize();
    virtual void finalize();
    virtual int do_work();
    virtual std::string prettyName();
};


#endif //TP1_RESULTSREPORTER_H
