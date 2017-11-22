#ifndef TP2_SERVICIO_H
#define TP2_SERVICIO_H

#include "ipc/SIGINT_Handler.h"
#include "ipc/Cola.h"
#include "Mensajes.h"
#include "Constantes.h"

class Servicio {
private:
    SIGINT_Handler sigint_handler;
    Cola<mensaje> _cola;
protected:
    int _tipoServicio;
    double parseDouble(std::string& string);
public:
    void ejecutar();
    virtual void dumpData() = 0;
    virtual std::string getDato(const std::string& key) = 0;
    Servicio(const Cola<mensaje>& cola, int tipo);
};

#endif //TP2_SERVICIO_H
