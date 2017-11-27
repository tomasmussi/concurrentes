#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include "ServicioMonedas.h"

void ServicioMonedas::dumpData() {
    if (DEBUG) {
        std::cout << "Guardando a disco la informacion del servicio de monedas" << std::endl;
    }
    std::ofstream outfile;
    outfile.open(ARCHIVO_MONEDAS);
    std::map<std::string, double>::iterator it = _data.begin();
    for ( ; it != _data.end(); ++it) {
        outfile << it->first << "," << it->second << "\n";
    }
    outfile.close();
}

ServicioMonedas::ServicioMonedas(const Cola<mensajeSS> &cola) : Servicio(cola, MONEDA) {
    std::ifstream infile(ARCHIVO_MONEDAS);
    // Si existe el archivo, lo levanto
    if (infile.good()) {
        std::string line;
        while (getline(infile, line).good()) {
            std::istringstream s(line);
            std::string record;
            std::vector<std::string> vector;
            while (getline(s, record, ',')) {
                vector.push_back(record);
            }
            _data[vector[0]] = parseDouble(vector[1]);
        }
    }
}

void ServicioMonedas::actualizarDato(char* dato) {
    std::string str(dato);
    size_t pos = str.find(' ');
    std::string key = str.substr(0, pos);
    std::transform(key.begin(), key.end(), key.begin(), ::toupper);
    std::string value_str = str.substr(pos + 1);
    double value = parseDouble(value_str);
    if (value > 0) {
        _data[key] = value;
    }
}

std::string ServicioMonedas::getDato(std::string &key) {
    std::transform(key.begin(), key.end(), key.begin(), ::toupper);
    if (_data.find(key) == _data.end()) {
        return "No se encontró cambio para la moneda " + key;
    }
    std::stringstream ss;
    ss << "El cambio de " << key << " es: 1 " << key << " = " << _data[key] << " ARS";
    return ss.str();
}
