//
// Created by tomas on 27/09/17.
//

#ifndef TP1_FIFO_H
#define TP1_FIFO_H

#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

class Fifo {
public:
    Fifo(const std::string nombre);
    virtual ~Fifo();
    virtual void abrir() = 0;
    void cerrar();
    void eliminar() const;

protected:
    std::string nombre;
    int fd;
};


#endif //TP1_FIFO_H
