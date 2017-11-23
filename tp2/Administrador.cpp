#include <cstring>
#include <iostream>
#include <vector>
#include <sstream>
#include "Administrador.h"
#include "Constantes.h"

Administrador::Administrador() : cola(MSG_ARCHIVO, CHAR_CLIENTE_SERVIDOR){}

void Administrador::ejecutar() {
    std::cout << "Primero indique qué tipo de información va actualizar. Para información asociada al estado del tiempo, utilice 1; para la cotización de una moneda 2." <<std::endl;
    std::cout << "Luego indique la ciudad o moneda que quiere actualizar. Por ejemplo, para una ciudad BSAS y para una moneda USD." <<std::endl;
    std::cout << "En el caso de la moneda a continuación indique el valor actualizado de la misma." <<std::endl;
    std::cout << "En el caso de la ciudad, ingrese los parámetros del estado del tiempo que quiere actualizar: Temperatura (t), presión atmosférica (p) y/o humedad (h). Cada una seguida de su correspondiente valor." <<std::endl;
    std::cout << "Ejemplos:" <<std::endl;
    std::cout << "Para actualizar la cotización del dólar: '2 USD 18.03'" <<std::endl;
    std::cout << "Para actualizar la temperatura de BSAS: '1 BSAS t 30.3'" <<std::endl;
    std::cout << "Para actualizar todos los parámetros del tiempo de Rosario: '1 ROSARIO t 25.2 p 1013 h 53.0'" << std::endl;
    std::cout << "Para finalizar ingrese únicamente la letra 'q'" << std::endl;

    bool quit = false;
    while (!quit) {
        std::cout << "Ingrese la información a actualizar: ";
        std::string line;
        std::getline (std::cin,line);
        if (line == "q") {
            quit = true;
            continue;
        }
        std::istringstream s(line);
        std::string record;
        std::vector<std::string> vector;
        while (getline(s, record, ' ')) {
            vector.push_back(record);
        }
        if ( vector[0] != "1" && vector[0] != "2" ){
            std::cout << "El primer número que ingrese debe ser un 1 para información asociada al estado del tiempo o un 2 para el valor de una moneda" << std::endl;
            continue;
        }
        int tipo = parse_int(vector[0]);
        if (vector[0] == "1"){
            //ESTADO DEL TIEMPO
            //TODO: Validar los valores ofrecidos
            if (vector.size() != 4 && vector.size() != 6 && vector.size() != 8 ){
                std::cout << "Cantidad de parámetros ingresada errónea. Deben ser 4,6 u 8 para la actualización del estado del tiempo " << std::endl;
                continue;
            }
        }
        else{
            //MONEDA
            //TODO: Validar los valores ofrecidos
            if (vector.size() != 3){
                std::cout << "Cantidad de parámetros ingresada errónea. Deben ser exactamente 3 para la cotización de una moneda " << std::endl;
                continue;
            }
        }
        std::string str = line.substr(2,std::string::npos);
        const char *request = str.c_str();

        mensajeCS m;
        m.mtype = NUEVA_CONEXION;
        cola.escribir(m);
        int resultado = cola.leer(RESPUESTA_NUEVA_CONEXION, &m);
        if (resultado != -1) {
            // El request va a ser a donde me indica el handShake
            int idCliente = m.id;
            m.mtype = idCliente;
            m.tipo = tipo;
            m.admin = true;
            strcpy(m.texto, request);
            cola.escribir(m);

            // Espero la respuesta en idCliente + 1
            mensajeCS rta;
            resultado = cola.leer(idCliente + 1, &rta);
            if (resultado != -1) {
                /*std::cout << "Administrador " << idCliente << " pidio por " << _tipoCliente << " " << _request <<
                    " y recibio '" << rta.texto << "'" << std::endl;*/
            }
        }
    }
}

int Administrador::parse_int(std::string str) {
    std::istringstream ss(str);
    int x;
    std::stringstream s;
    if (!(ss >> x) and x != MONEDA and x != TIEMPO) {
        std::cout << "Tipo de cliente no reconocido. Debe ser " << TIEMPO << " (Tiempo) o bien " << MONEDA << " (Moneda)" << std::endl;
        return -1;
    }
    return x;
}
