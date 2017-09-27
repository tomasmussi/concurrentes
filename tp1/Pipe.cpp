//
// Created by tomas on 27/09/17.
//

#include "Pipe.h"

Pipe :: Pipe() : mode_read(true), mode_write(true) {
    pipe ( this->descriptors );
}

Pipe::~Pipe() {
}

Pipe::Pipe(const Pipe& other_pipe) {
    this->mode_write = other_pipe.mode_write;
    this->mode_read = other_pipe.mode_read;
    this->descriptors[MODE_READ] = other_pipe.descriptors[MODE_READ];
    this->descriptors[MODE_WRITE] = other_pipe.descriptors[MODE_WRITE];
}

Pipe Pipe::operator=(const Pipe& other_pipe) {
    this->mode_write = other_pipe.mode_write;
    this->mode_read = other_pipe.mode_read;
    this->descriptors[MODE_READ] = other_pipe.descriptors[MODE_READ];
    this->descriptors[MODE_WRITE] = other_pipe.descriptors[MODE_WRITE];
    return *this;
}

void Pipe :: set_mode ( const int mode ) {
    if ( mode == MODE_READ ) {
        close ( this->descriptors[MODE_WRITE] );
        this->mode_write = false;

    } else if ( mode == MODE_WRITE ) {
        close ( this->descriptors[MODE_READ] );
        this->mode_read = false;
    }
}

ssize_t Pipe :: write_pipe ( const void* dato,int datoSize ) {
    if ( this->mode_read ) {
        close ( this->descriptors[MODE_READ] );
        this->mode_read = false;
    }
    return write ( this->descriptors[MODE_WRITE],dato,datoSize );
}

ssize_t Pipe :: read_pipe ( void* buffer,const int buffSize ) {
    if ( this->mode_write ) {
        close ( this->descriptors[MODE_WRITE] );
        this->mode_write = false;
    }
    return read ( this->descriptors[MODE_READ],buffer,buffSize );
}

int Pipe ::get_fd_read() const {
    if ( this->mode_read )
        return this->descriptors[MODE_READ];
    else
        return -1;
}

int Pipe ::get_fd_write() const {
    if ( this->mode_write )
        return this->descriptors[MODE_WRITE];
    else
        return -1;
}

void Pipe ::close_fifo() {
    if ( this->mode_read ) {
        close ( this->descriptors[MODE_READ] );
        this->mode_read = false;
    }
    if ( this->mode_write ) {
        close ( this->descriptors[MODE_WRITE] );
        this->mode_write = false;
    }
}