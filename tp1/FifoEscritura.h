//
// Created by tomas on 27/09/17.
//

#ifndef TP1_FIFOESCRITURA_H
#define TP1_FIFOESCRITURA_H

#include "Fifo.h"

class FifoEscritura : Fifo {
public:
    FifoEscritura(const std::string nombre);
    ~FifoEscritura();

    void abrir();
    ssize_t escribir(const void* buffer,const ssize_t buffsize) const;
};


#endif //TP1_FIFOESCRITURA_H
