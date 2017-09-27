//
// Created by tomas on 27/09/17.
//

#ifndef TP1_PIPE_H
#define TP1_PIPE_H


#include <unistd.h>
#include <fcntl.h>

class Pipe {
private:
    int descriptors[2];
    bool mode_read;
    bool mode_write;

public:
    static const int MODE_READ = 0;
    static const int MODE_WRITE = 1;

    Pipe();
    Pipe(const Pipe& other_pipe);
    Pipe operator=(const Pipe& other_pipe);
    ~Pipe();

    void set_mode ( const int modo );

    ssize_t write_pipe ( const void* dato,const int datoSize );
    ssize_t read_pipe ( void* buffer,const int buffSize );

    int get_fd_read () const;
    int get_fd_write () const;

    void close_fifo ();
};


#endif //TP1_PIPE_H
