#ifndef TP1_TIMER_H
#define TP1_TIMER_H

#include "WorkerProcess.h"

class Timer : public WorkerProcess, EventHandler {
private:
    bool already_finished;
public:
    Timer();
    virtual ~Timer();
    virtual void initialize();
    virtual void finalize();
    virtual int do_work();
    virtual std::string prettyName();
    virtual int handleSignal(int signum);
};

#endif //TP1_TIMER_H
