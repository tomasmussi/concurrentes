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

class CourtManager : public WorkerProcess, public EventHandler {
private:
    int _m;
    int _k;
    int _rows;
    int _columns;
    FifoLectura _fifo_read;
    FifoEscritura _fifo_write;
    LockFile _lock_shm_mapper;
    MemoriaCompartida<int>* _shm_mapper;
    LockFile _lock_shm_player_couple;
    MemoriaCompartida<int>* _shm_player_couple;
    LockFile _lock_matches;
    MemoriaCompartida<int> _shm_matches;
    std::map<pid_t, Match> _matches;

    void initialize_shm_couples();
    void destroy_shm_couples();
    void initialize_shm_mapper();
    void destroy_shm_mapper();
    int get_shm_index(int row, int col);
    int lookup(const Person& person);
    void write_shm_mapper(int idx_p1, int idx_p2);
    void process_finished_match();
    void dispatch_match(const Team& team1, const Team& team2);
public:
    CourtManager(int m, int k,int rows, int columns, const std::string& fifo_read,
                 const std::string& fifo_write);
    ~CourtManager();
    virtual void initialize();
    virtual int do_work();
    virtual void finalize();
    virtual std::string prettyName();
    virtual int handleSignal ( int signum );
};

#endif //TP1_COURTMANAGER_H