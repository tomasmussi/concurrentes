#include <iostream>
#include <fstream>
#include <sstream>
#include "ServicioMonedas.h"

void ServicioMonedas::dumpData() {
    std::cout << "Guardando a disco la informacion del servicio de monedas" << std::endl;
}

ServicioMonedas::ServicioMonedas(const Cola<mensaje> &cola) : Servicio(cola, MONEDA) {
//    std::ifstream infile(ARCHIVO_MONEDAS);
    // Si existe el archivo, lo levanto
//    if (infile.good()) {
//        // TODO: Leer archivo
//    } else {
    _data["USD"] = 1/18.5;
//    }
}

std::string ServicioMonedas::getDato(const std::string &key) {
    if (_data.find(key) == _data.end()) {
        return "No se encontró cambio para la moneda " + key;
    }
    std::stringstream ss;
    ss << "El cambio de " << key << " es: 1 ARS = " << _data[key] << " " << key;
    return ss.str();
}
