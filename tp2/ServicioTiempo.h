#ifndef TP2_SERVICIOTIEMPO_H
#define TP2_SERVICIOTIEMPO_H

#include <map>
#include "Servicio.h"

typedef struct tiempo {
    double temperatura;
    int presion;
    double humedad;
} tiempo;

class ServicioTiempo : public Servicio {
private:
    std::map<std::string, tiempo> _data;
protected:
    int _tipoServicio;
public:
    ServicioTiempo(const Cola<mensajeSS> &cola);

    virtual void dumpData();
    virtual std::string getDato(const std::string& key);
    virtual void actualizarDato(char* data);
};

#endif //TP2_SERVICIOTIEMPO_H
