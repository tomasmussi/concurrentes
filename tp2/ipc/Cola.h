#ifndef TP2_COLA_H
#define TP2_COLA_H

#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <string>
#include <cerrno>

template <class T> class Cola {
private:
    key_t   clave;
    int     id;

public:
    Cola(const std::string& archivo, const char letra);
    ~Cola();
    int escribir(const T& dato) const;
    int leer(const int tipo, T* buffer) const;
    int destruir() const;
};

template <class T> Cola<T>::Cola (const std::string& archivo, const char letra) {
    this->clave = ftok(archivo.c_str(), letra);
    if (this->clave == -1)
        perror("Error en ftok");

    this->id = msgget(this->clave, 0777|IPC_CREAT);
    if (this->id == -1)
        perror("Error en msgget");
}

template <class T> Cola<T>::~Cola() {
}

template <class T> int Cola<T>::destruir() const {
    int resultado = msgctl(this->id, IPC_RMID, NULL);
    if (resultado == -1)
        perror("Error al destruir cola");
    return resultado;
}

template <class T> int Cola<T>::escribir(const T& dato) const {
    int resultado = msgsnd(this->id, static_cast<const void*>(&dato), sizeof(T) - sizeof(long), 0);
    if (resultado == -1 and errno != EINTR)
        perror("Error al escribir en cola");
    return resultado;
}

template <class T> int Cola<T>::leer(const int tipo, T* buffer) const {
    int resultado = msgrcv(this->id, static_cast<void *>(buffer), sizeof(T) - sizeof(long), tipo, 0);
    if (resultado == -1 and errno != EINTR)
        perror("Error al leer de la cola");
    return resultado;
}

#endif //TP2_COLA_H
