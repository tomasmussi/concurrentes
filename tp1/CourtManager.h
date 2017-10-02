//
// Created by tomas on 01/10/17.
//

#ifndef TP1_COURTMANAGER_H
#define TP1_COURTMANAGER_H

#include <string>
#include "WorkerProcess.h"
#include "MemoriaCompartida.h"
#include "FifoLectura.h"


class CourtManager : public WorkerProcess {
private:
    int _m;
    int _k;
    int _rows;
    int _columns;
    FifoLectura _fifo_read;
    MemoriaCompartida<int>* _shm_player_couple;

    void initialize_shm();
    void destroy_shm();
    int get_shm_index(int row, int col);
public:
    CourtManager(int m, int k,int rows, int columns, const std::string& fifo_read);
    ~CourtManager();
    virtual void initialize();
    virtual int do_work();
    virtual void finalize();
    virtual std::string prettyName();
};


#endif //TP1_COURTMANAGER_H
