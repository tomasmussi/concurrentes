//
// Created by tomas on 29/09/17.
//

#ifndef TP1_TEAMMAKER_H
#define TP1_TEAMMAKER_H

#include <string>
#include <list>
#include <map>
#include "WorkerProcess.h"
#include "FifoLectura.h"
#include "FifoEscritura.h"
#include "MemoriaCompartida.h"
#include "Person.h"

class TeamMaker : public WorkerProcess {
private:
    int _m;
    int _k;
    FifoLectura _fifo_read;
    FifoEscritura _fifo_write;
    MemoriaCompartida<int>* _shm_player_couple;
    Person _some_player;
    bool _have_player;
    std::map<std::string, int> _id_mapper;
    std::list<int> _free_ids;

    std::list<Person> _waiting_list;

    FifoEscritura _fifo_prueba; // TODO ELIMINAR!!!!

    void initialize_shm();
    void destroy_shm();
    int get_shm_index(int row, int col);

public:
    TeamMaker(int m, int k, const std::string& fifo_read, const std::string& fifo_write);
    virtual void initialize();
    virtual int do_work();
    virtual void finalize();
    virtual std::string prettyName();
};


#endif //TP1_TEAMMAKER_H
