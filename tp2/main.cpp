#include <iostream>
#include <cstring>

#include "Cola.h"
#include "Mensajes.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    Cola<mensaje> c("/bin/bash", 'a');
    mensaje m;
    m.id = 4;
    m.mtype = PETICION;
    strcpy(m.texto, "asdasd");
    c.escribir(m);
    mensaje rta;
    c.leer(PETICION, &rta);
    std::cout << "sacado de cola: " << m.id << ", " << m.mtype << ", " << m.texto << std::endl;
    c.destruir();
    return 0;
}