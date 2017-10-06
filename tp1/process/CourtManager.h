#ifndef TP1_COURTMANAGER_H
#define TP1_COURTMANAGER_H

#include <string>
#include "WorkerProcess.h"
#include "../ipc/MemoriaCompartida.h"
#include "../ipc/FifoLectura.h"
#include "MatchProcess.h"
#include "../handlers/EventHandler.h"
#include "../ipc/FifoEscritura.h"
#include "../ipc/LockFile.h"
#include "../model/Match.h"
#include "../ipc/Semaphore.h"

class CourtManager : public WorkerProcess, public EventHandler {
private:
    int _m;
    int _k;
    int _rows;
    int _columns;
    FifoLectura _fifo_read;

    FifoEscritura _fifo_write_people;
    FifoEscritura _fifo_write_matches;
    FifoEscritura _fifo_write;

    LockFile _lock_matches;
    MemoriaCompartida<int> _shm_matches;
    std::map<pid_t, Match> _matches;
    Semaphore _available_courts;

    void process_finished_match();
    void dispatch_match(const Team& team1, const Team& team2);
public:
    CourtManager(int m, int k,int rows, int columns, const std::string& fifo_read,
                 const std::string& fifo_write_people, const std::string& fifo_write_matches );
    ~CourtManager();
    virtual void initialize();
    virtual int do_work();
    virtual void finalize();
    virtual std::string prettyName();
    virtual int handleSignal ( int signum );
};

#endif //TP1_COURTMANAGER_H
