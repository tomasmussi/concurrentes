#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "ServicioTiempo.h"

void ServicioTiempo::dumpData() {
    if (DEBUG) {
        std::cout << "Guardando a disco la informacion del servicio de tiempo" << std::endl;
    }
    std::ofstream outfile;
    outfile.open(ARCHIVO_TIEMPO);
    std::map<std::string, tiempo>::iterator it = _data.begin();
    for ( ; it != _data.end(); ++it) {
        outfile << it->first << "," << it->second.temperatura << "," << it->second.presion << "," << it->second.humedad << "\n";
    }
    outfile.close();
}

ServicioTiempo::ServicioTiempo(const Cola<mensaje> &cola) : Servicio(cola, TIEMPO) {
    std::ifstream infile(ARCHIVO_TIEMPO);
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
            struct tiempo t;
            t.temperatura = parseDouble(vector[1]);
            t.presion = static_cast<int>(parseDouble(vector[2]));
            t.humedad = parseDouble(vector[3]);
            _data[vector[0]] = t;
        }
    }
}

std::string ServicioTiempo::getDato(const std::string &key) {
    if (_data.find(key) == _data.end()) {
        return "No se encontró tiempo para la ciudad " + key;
    }
    tiempo t = _data[key];
    std::stringstream ss;
    ss << "El tiempo de " << key << " es: " << std::endl <<
        "Temperatura: " << t.temperatura << "°C" << std::endl <<
        "Presion: " << t.presion << "hPa" << std::endl <<
        "Humedad: " << t.humedad << "%";
    return ss.str();
}

void ServicioTiempo::actualizarDato(mensaje msg) {
    if (DEBUG) {
        std::cout << "Actualizando [" << msg.texto << "], temp: " << msg.tiemp.temperatura
           << ", presion: " << msg.tiemp.presion << ", humedad: " << msg.tiemp.humedad << std::endl;
    }
    _data[msg.texto] = msg.tiemp;
}