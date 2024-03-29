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
    FifoLectura _fifo_read_couples;

    // Este fifo se usa para escribir las parejas inundadas de nuevo para que puedan jugar
    FifoEscritura _fifo_write_couples;
    FifoEscritura _fifo_write_people;
    FifoEscritura _fifo_write_matches;

    std::map<pid_t, Match> _matches;
    Semaphore _available_courts;
    std::map<int, std::map<int, int> > _court_state; // Estado de cancha
    std::map<int, std::map<int, pid_t > > _court_pid; // Que proceso esta en que cancha
    int _tide_column;
    pid_t timer_pid;

    bool occupy_court(pid_t pid);
    bool free_court(pid_t pid);

    void handle_matches(int signum);
    void kill_matches();
    void tide_rise(int signum);
    void tide_decrease(int signum);

    void process_finished_match(pid_t match_pid, int status);
    void dispatch_match(const Team& team1, const Team& team2);
public:
    CourtManager(int m, int k,int rows, int columns, const std::string& fifo_couples,
                 const std::string& fifo_write_people, const std::string& fifo_write_matches);
    ~CourtManager();
    virtual void initialize();
    virtual int do_work();
    virtual void finalize();
    virtual std::string prettyName();
    virtual int handleSignal(int signum);
    virtual void setTimerPid(pid_t pid);
};

#endif //TP1_COURTMANAGER_H
