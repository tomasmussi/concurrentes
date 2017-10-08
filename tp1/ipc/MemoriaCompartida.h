#ifndef TP1_MEMORIACOMPARTIDA_H
#define TP1_MEMORIACOMPARTIDA_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string>
#include <string.h>
#include <iostream>
#include <errno.h>
#include "../utils/Logger.h"

#include <sstream>

template <class T> class MemoriaCompartida {

private:
    int shmId;
    T* ptrDatos;

    int cantidadProcesosAdosados() const;

public:
    MemoriaCompartida();
    ~MemoriaCompartida();
    void crear(const std::string& archivo, const char letra);
    void liberar();
    void escribir(const T& dato);
    T leer() const;
};

template <class T> MemoriaCompartida<T>::MemoriaCompartida() : shmId(0), ptrDatos(NULL) {
}

template <class T> MemoriaCompartida<T>::~MemoriaCompartida() {
}

template <class T> void MemoriaCompartida<T>::crear(const std::string& archivo, const char letra) {
    key_t clave = ftok(archivo.c_str(), letra);

    if (clave > 0) {
        this->shmId = shmget(clave, sizeof(T), 0644|IPC_CREAT);

        if (this->shmId > 0) {
            void* tmpPtr = shmat(this->shmId, NULL, 0);
            if (tmpPtr != (void*) -1) {
                this->ptrDatos = static_cast<T*>(tmpPtr);
            } else {
                Logger::log("SHM", Logger::WARNING, "Error al crear 1", Logger::get_date());
                std::string mensaje = std::string("Error en shmat(): ") + std::string(strerror(errno));
                throw mensaje;
            }
        } else {
            Logger::log("SHM", Logger::WARNING, "Error al crear 2", Logger::get_date());
            std::string mensaje = std::string("Error en shmget(): ") + std::string(strerror(errno));
            throw mensaje;
        }
    } else {
        Logger::log("SHM", Logger::WARNING, "Error al crear 3", Logger::get_date());
        std::string mensaje = std::string("Error en ftok(): ") + std::string(strerror(errno));
        throw mensaje;
    }
}


template <class T> void MemoriaCompartida<T>::liberar() {
    int errorDt = shmdt((void *)this->ptrDatos);

    if (errorDt != -1) {
        int procAdosados = this->cantidadProcesosAdosados();
        if (procAdosados == 0) {
            shmctl(this->shmId, IPC_RMID, NULL);
        }
    } else {
        std::stringstream s;
        s << "[" << getpid() << "] Error al liberar: " << std::string(strerror(errno));
        Logger::log("SHM", Logger::WARNING, s.str(), Logger::get_date());
        std::string mensaje = std::string("Error en shmdt(): ") + std::string(strerror(errno));
        throw mensaje;
    }
}

template <class T> void MemoriaCompartida<T>::escribir(const T& dato) {
    *(this->ptrDatos) = dato;
}

template <class T> T MemoriaCompartida<T>::leer() const {
    return *(this->ptrDatos);
}

template <class T> int MemoriaCompartida<T>::cantidadProcesosAdosados() const {
    shmid_ds estado;
    shmctl(this->shmId, IPC_STAT, &estado);
    return estado.shm_nattch;
}

#endif //TP1_MEMORIACOMPARTIDA_H
