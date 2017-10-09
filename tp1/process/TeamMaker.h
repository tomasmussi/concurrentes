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
#include "../ipc/Semaphore.h"
#include "../ipc/MemoriaCompartida.h"

#define MAX_GONE_PLAYERS 128
// Probabilidad de que un jugador decida irse
#define LEAVE_PROBABILITY 20

class TeamMaker : public WorkerProcess {
private:
    int _k;
    FifoLectura _fifo_read;
    FifoEscritura _fifo_write;
    LockFile _lock_shm_gone_players;
    MemoriaCompartida<int>* _shm_gone_players;
    Semaphore _semaphore;
    bool _have_player;
    std::map<std::string,std::map<std::string,bool> > _couples; //True si ya fueron pareja
    std::list<Person> _waiting_list;

    bool can_play(std::string id_p1, std::string id_p2);
    void add_couple_to_map(std::string id_p1, std::string id_p2);
    void new_id_into_map(std::string id);
    int matches_played(std::string id);

    void initialize_shm_gone_players();
    void destroy_shm_gone_players();
    void write_shm_gone_players(int id);
    int _last_gone;

public:
    TeamMaker(int k, const std::string& fifo_read, const std::string& fifo_write, Semaphore& semaphore);
    virtual void initialize();
    virtual int do_work();
    virtual void finalize();
    virtual std::string prettyName();
};

#endif //TP1_TEAMMAKER_H
