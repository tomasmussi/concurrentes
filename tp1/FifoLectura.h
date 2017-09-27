//
// Created by tomas on 27/09/17.
//

#ifndef TP1_FIFOLECTURA_H
#define TP1_FIFOLECTURA_H

#include "Fifo.h"

class FifoLectura : public Fifo {
public:
    FifoLectura(const std::string nombre);
    ~FifoLectura();

    void abrir();
    ssize_t leer(void* buffer,const ssize_t buffsize) const;
};


#endif //TP1_FIFOLECTURA_H
