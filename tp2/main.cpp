#include <iostream>
#include <cstring>

#include "ipc/Cola.h"
#include "Mensajes.h"
#include "Servidor.h"

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cout << "Debe especificar cliente o servidor" << std::endl;
        std::cout << "cliente: \'c\' 1 BSAS" << std::endl;
        std::cout << "cliente: \'c\' 2 USD" << std::endl;
        std::cout << "servidor: \'s\'" << std::endl;
        return 1;
    }
    char opt = argv[1][0];
    if (opt == 'c') {
        // Cliente
        Cola<mensaje> cola(MSG_ARCHIVO, CHAR_CLIENTE_SERVIDOR);
        mensaje m;
        m.mtype = TIEMPO;
        m.id = 3;
        strcpy(m.texto, "Consultando por BSAS");
        cola.escribir(m);
        mensaje rta;
        cola.leer(m.id, &rta);
        std::cout << rta.texto << std::endl;

        m.mtype = MONEDA;
        m.id = 4;
        strcpy(m.texto, "Consultando por USD");
        cola.escribir(m);
        cola.leer(m.id, &rta);
        std::cout << rta.texto << std::endl;


    } else if(opt == 's') {
        // Servidor
        Servidor s;
        s.ejecutar();
    } else {
        std::cout << "Opcion no reconocida" << std::endl;
        std::cout << "cliente: \'c\' 1 BSAS" << std::endl;
        std::cout << "cliente: \'c\' 2 USD" << std::endl;
        std::cout << "servidor: \'s\'" << std::endl;
        return 1;
    }
    return 0;
}