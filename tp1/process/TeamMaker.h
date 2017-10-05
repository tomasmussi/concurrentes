#ifndef TP1_TEAMMAKER_H
#define TP1_TEAMMAKER_H

#include <string>
#include <list>
#include <map>
#include "WorkerProcess.h"
#include "../ipc/FifoLectura.h"
#include "../ipc/FifoEscritura.h"
#include "../ipc/MemoriaCompartida.h"
#include "../model/Person.h"
#include "../ipc/LockFile.h"

class TeamMaker : public WorkerProcess {
private:
    int _m;
    int _k;
    FifoLectura _fifo_read;
    FifoEscritura _fifo_write;
    LockFile _lock_shm_mapper;
    MemoriaCompartida<int>* _shm_mapper;
    LockFile _lock_shm_player_couple;
    MemoriaCompartida<int>* _shm_player_couple;
    bool _have_player;
    std::map<std::string, int> _id_mapper;
    std::list<Person> _waiting_list;

    void initialize_shm();
    void destroy_shm();
    void initalize_shm_mapper();
    void destroy_shm_mapper();
    int get_shm_index(int row, int col);
    int lookup(const Person& person);
    int find_empty_space(const Person& person);
    bool can_play(int shm_id, const Person& person);

public:
    TeamMaker(int m, int k, const std::string& fifo_read, const std::string& fifo_write);
    virtual void initialize();
    virtual int do_work();
    virtual void finalize();
    virtual std::string prettyName();
};

#endif //TP1_TEAMMAKER_H
