#include <iostream>
#include <fstream>
#include <sstream>
#include "ServicioTiempo.h"

void ServicioTiempo::dumpData() {
    std::cout << "Guardando a disco la informacion del servicio de tiempo" << std::endl;
}

ServicioTiempo::ServicioTiempo(const Cola<mensaje> &cola) : Servicio(cola, TIEMPO) {
//    std::ifstream infile(ARCHIVO_TIEMPO);
    // Si existe el archivo, lo levanto
//    if (infile.good()) {
//        // TODO: Leer archivo
//    } else {
    struct tiempo t;
    t.temperatura = 18.5;
    t.humedad = 85.2;
    t.presion = 1020;
    _data["BSAS"] = t;
//    }
}

std::string ServicioTiempo::getDato(const std::string &key) {
    tiempo t = _data[key];
    std::stringstream ss;
    ss << "El tiempo de " << key << " es: " << std::endl <<
        "Temperatura: " << t.temperatura << "°C" << std::endl <<
        "Presion: " << t.presion << "hPa" << std::endl <<
        "Humedad: " << t.humedad << "%";
    return ss.str();
}
