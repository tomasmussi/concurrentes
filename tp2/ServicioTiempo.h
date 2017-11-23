#ifndef TP2_SERVICIOTIEMPO_H
#define TP2_SERVICIOTIEMPO_H

#include <map>
#include "Servicio.h"



class ServicioTiempo : public Servicio {
private:
    std::map<std::string, tiempo> _data;
protected:
    int _tipoServicio;
    virtual void actualizarDato(mensaje msg);
public:
    ServicioTiempo(const Cola<mensaje> &cola);

    virtual void dumpData();
    virtual std::string getDato(const std::string& key);
};

#endif //TP2_SERVICIOTIEMPO_H
