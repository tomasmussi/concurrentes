#include <iostream>
#include <algorithm>
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

ServicioTiempo::ServicioTiempo(const Cola<mensajeSS> &cola) : Servicio(cola, TIEMPO) {
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

void ServicioTiempo::actualizarDato(char* data) {
    std::string str(data);
    std::istringstream s(str);
    std::string record;
    std::vector<std::string> vector;
    while (getline(s, record, ' ')) {
        vector.push_back(record);
    }
    std::string key = vector[0];
    std::transform(key.begin(), key.end(), key.begin(), ::toupper);
    struct tiempo t;
    if (_data.count(key) == 0) {
        // Inicializar todos los valores de alguna forma inválida ya que podrían no agregarse todos sus parametros
        t.temperatura = -274.0;
        t.presion = -1;
        t.humedad = -1.0;
    } else {
        t = _data[key];
    }
    size_t pos = 1;
    while (pos < vector.size() - 1) {
        std::string param = vector[pos];
        double value = parseDouble(vector[pos+1]);
        if (param == "t") {
            t.temperatura = value;
        } else if (param == "p") {
            t.presion = static_cast<int>(value);
        } else if (param == "h") {
            t.humedad = value;
        }
        pos += 2;
    }
    _data[key] = t;
}

std::string ServicioTiempo::getDato(std::string &key) {
    std::transform(key.begin(), key.end(), key.begin(), ::toupper);
    if (_data.find(key) == _data.end()) {
        return "No se encontró tiempo para la ciudad " + key;
    }
    tiempo t = _data[key];
    std::stringstream ss;
    ss << "El tiempo de " << key << " es: " << std::endl;
    if (t.temperatura < -273) {
        ss << "Temperatura: Desconocida"<< std::endl;
    } else {
        ss << "Temperatura: " << t.temperatura << "°C" << std::endl;
    }
    if (t.presion < 0) {
        ss << "Presión: Desconocida"<< std::endl;
    } else {
        ss << "Presión: " << t.presion << "hPa" << std::endl;
    }
    if (t.humedad < 0) {
        ss << "Humedad: Desconocida";
    } else {
        ss << "Humedad: " << t.humedad << "%";
    }
    return ss.str();
}
