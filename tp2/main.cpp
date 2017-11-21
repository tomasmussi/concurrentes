#include <iostream>
#include <cstring>

#include "Servidor.h"
#include "Cliente.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Debe especificar cliente o servidor de la siguiente manera:" << std::endl;
        std::cout << "Cliente de clima: 'c 1 BSAS'" << std::endl;
        std::cout << "Cliente de monedas: 'c 2 USD'" << std::endl;
        std::cout << "Servidor: 's'" << std::endl;
        return -1;
    }
    char opt = argv[1][0];
    if (opt == 'c') {
        // Cliente
        Cliente c;
        c.ejecutar();
    } else if (opt == 's') {
        // Servidor
        Servidor s;
        s.ejecutar();
    } else {
        std::cout << "Opcion no reconocida. Debe ejecutar de la siguiente manera:" << std::endl;
        std::cout << "Cliente de clima: 'c 1 BSAS'" << std::endl;
        std::cout << "Cliente de monedas: 'c 2 USD'" << std::endl;
        std::cout << "Servidor: 's'" << std::endl;
        return -2;
    }
    return 0;
}