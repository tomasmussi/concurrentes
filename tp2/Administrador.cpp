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
        int tipo = parse_int(vector[0]);
        if ((tipo == INVALIDO) or ((tipo != MONEDA) and (tipo != TIEMPO))){
            std::cout << "El primer parámetro debe ser un número entero y debe tomar el valor " << TIEMPO << " (Tiempo) o el valor " << MONEDA << " (Moneda)" << std::endl;
            continue;
        }
        if (tipo == 1){
            //ESTADO DEL TIEMPO
            //TODO: Validar los valores ofrecidos
            if (vector.size() != 4 && vector.size() != 6 && vector.size() != 8 ){
                std::cout << "Cantidad de parámetros ingresada errónea. Deben ser 4,6 u 8 para la actualización del estado del tiempo " << std::endl;
                continue;
            }
            if(!validar_parametros_tiempo(&vector)){
                continue;
            }
        }
        else{
            //MONEDA
            if (vector.size() != 3){
                std::cout << "Cantidad de parámetros ingresada errónea. Deben ser exactamente 3 para la cotización de una moneda " << std::endl;
                continue;
            }
            if (parse_double(vector[2]) < 0){
                std::cout << "El valor de la cotización debe ser numérico positivo. Ejemplo de formato de número flotante: 17.23" << std::endl;
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
    if (!(ss >> x)) {
        return INVALIDO;
    }
    return x;
}

double Administrador::parse_double(std::string& string) {
    std::istringstream ss(string);
    double x;
    if (!(ss >> x)) {
        return INVALIDO;
    }
    return x;
}

bool Administrador::validar_parametros_tiempo(std::vector<std::string>* vector){
    int t = 0, p = 0, h = 0;
    size_t posT = -1, posP = -1, posH = -1;
    size_t pos = 2;
    while (pos < vector->size() - 1){
        if(vector->at(pos) == TEMPERATURA){
            t++;
            posT = pos;
        }
        else if (vector->at(pos) == PRESION){
            p++;
            posP = pos;
        }
        else if (vector->at(pos) == HUMEDAD){
            h++;
            posH = pos;
        }
        else{
            std::cout << "Parámetro inválido: debe ser " << TEMPERATURA << " para la Temperatura, ";
            std::cout << PRESION << " para la Presión y " << HUMEDAD << " para la Humedad." << std::endl;
            return false;
        }
        pos += 2;
    }
    if (t > 1 or p > 1 or h > 1){
        std::cout << "Parámetro repetido: cada parámetro del estado del tiempo puede usarse una única vez"<< std::endl;
        return false;
    }
    // Acá ya estoy seguro que los nombres de los parámetros son correctos y que no se repitieron (como mucho están una vez)
    if (t > 0){
        double temp = parse_double(vector->at(posT + 1));
        if( temp == INVALIDO or temp < -273 ){
            std::cout << "Valor inválido para la Temperatura, debe ser un número flotante mayor a -273°C"<< std::endl;
            return false;
        }
    }
    if (p > 0){
        int presion = parse_int(vector->at(posP + 1));
        if( presion < 0 ){
            std::cout << "Valor inválido para la Presión, debe ser un número entero positivo (hPa)"<< std::endl;
            return false;
        }
    }
    if (h > 0){
        double humedad = parse_double(vector->at(posH + 1));
        if( humedad < 0 or humedad > 100 ){
            std::cout << "Valor inválido para la Humedad, debe ser un número flotante entre 0 y 100 (%)"<< std::endl;
            return false;
        }
    }
    return true;
}
