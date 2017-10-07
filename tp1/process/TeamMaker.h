#ifndef TP1_TEAMMAKER_H
#define TP1_TEAMMAKER_H

#include <string>
#include <list>
#include <map>
#include "WorkerProcess.h"
#include "../ipc/FifoLectura.h"
#include "../ipc/FifoEscritura.h"
#include "../model/Person.h"
#include "../ipc/LockFile.h"

class TeamMaker : public WorkerProcess {
private:
    int _m;
    int _k;
    FifoLectura _fifo_read;
    FifoEscritura _fifo_write;
    bool _have_player;
    std::map<std::string,std::map<std::string,bool> > _couples; //True si ya fueron pareja
    std::list<Person> _waiting_list;

    bool can_play(std::string id_p1, std::string id_p2);
    void add_couple_to_map(std::string id_p1, std::string id_p2);
    void new_id_into_map(std::string id);

public:
    TeamMaker(int m, int k, const std::string& fifo_read, const std::string& fifo_write);
    virtual void initialize();
    virtual int do_work();
    virtual void finalize();
    virtual std::string prettyName();
};

#endif //TP1_TEAMMAKER_H
