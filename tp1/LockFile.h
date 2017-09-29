//
// Created by tomas on 29/09/17.
//

#ifndef TP1_LOCKFILE_H
#define TP1_LOCKFILE_H

#include <unistd.h>
#include <fcntl.h>
#include <string>

class LockFile {

private:
    struct flock fl;
    int fd;
    std::string nombre;

public:
    LockFile(const std::string& nombre);
    ~LockFile();
    int lock();
    int release();
    ssize_t write_lock(const void* buffer, const ssize_t buffsize) const;

};


#endif //TP1_LOCKFILE_H
