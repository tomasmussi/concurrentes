#ifndef TP1_TIMER_H
#define TP1_TIMER_H

#include "WorkerProcess.h"
#include "../ipc/Semaphore.h"

class Timer : public WorkerProcess, EventHandler {
private:
    Semaphore _tournament_started;
    bool _already_finished;
public:
    Timer(Semaphore& tournament_started);
    virtual ~Timer();
    virtual void initialize();
    virtual void finalize();
    virtual int do_work();
    virtual std::string prettyName();
    virtual int handleSignal(int signum);
};

#endif //TP1_TIMER_H
