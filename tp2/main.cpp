#include <iostream>
#include <cstring>
#include <sstream>

#include "Servidor.h"
#include "Cliente.h"

int parse_int(char* arg) {
    std::istringstream ss(arg);
    int x;
    std::stringstream s;
    if (!(ss >> x) and x != MONEDA and x != TIEMPO) {
        std::cout << "Tipo de cliente no reconocido. Debe ser " << TIEMPO << " (Tiempo) o bien " << MONEDA << " (Moneda)" << std::endl;
        return -1;
    }
    return x;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Debe especificar cliente o servidor de la siguiente manera:" << std::endl;
        std::cout << "Cliente de clima: 'c 1 BSAS'" << std::endl;
        std::cout << "Cliente de monedas: 'c 2 USD'" << std::endl;
        std::cout << "Servidor: 's'" << std::endl;
        std::cout << "Administrador de clima: 'c 1 BSAS'" << std::endl;
        std::cout << "Administrador de monedas: 'c 2 USD'" << std::endl;
        return -1;
    }
    char opt = argv[1][0];
    if (opt == 'c') {
        // Cliente
        if (argc != 4) {
            std::cout << "Para ejecutar un cliente debe especificar qué tipo de consulta se quiere hacer de la siguiente manera:" << std::endl;
            std::cout << "Cliente de clima: 'c 1 BSAS'" << std::endl;
            std::cout << "Cliente de monedas: 'c 2 USD'" << std::endl;
            return -3;
        }
        int tipoCliente = parse_int(argv[2]);
        if (tipoCliente == -1) return -4;
        Cliente c(tipoCliente, argv[3]);
        c.ejecutar();
    } else if (opt == 's') {
        // Servidor
        Servidor s;
        s.ejecutar();
    } else if (opt == 'a') {
        // Administrador
        if (argc != 5 && argc != 7) {
            std::cout << "Para ejecutar administrador debe especificar qué tipo de consulta se quiere hacer de la siguiente manera:" << std::endl;
            std::cout << "Administrador de clima: 'a 1 BSAS 25 1014 75'" << std::endl;
            std::cout << "Administrador de monedas: 'a 2 USD 0.056'" << std::endl;
            return -5;
        }
        int tipoCliente = parse_int(argv[2]);
        if (tipoCliente == -1) return -6;
        Cliente c(tipoCliente, argv);
        c.ejecutar();
    } else {
        std::cout << "Opcion no reconocida. Debe ejecutar de la siguiente manera:" << std::endl;
        std::cout << "Cliente de clima: 'c 1 BSAS'" << std::endl;
        std::cout << "Cliente de monedas: 'c 2 USD'" << std::endl;
        std::cout << "Servidor: 's'" << std::endl;
        return -2;
    }
    return 0;
}