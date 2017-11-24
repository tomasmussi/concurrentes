#ifndef TP2_SERVICIOMONEDAS_H
#define TP2_SERVICIOMONEDAS_H

#include <map>
#include "Servicio.h"

class ServicioMonedas : public Servicio {
private:
    std::map<std::string, double> _data;
protected:
    int _tipoServicio;
public:
    ServicioMonedas(const Cola<mensajeSS> &cola);

    virtual void dumpData();
    virtual std::string getDato(const std::string& key);
    virtual void actualizarDato(char* data);
};

#endif //TP2_SERVICIOMONEDAS_H
