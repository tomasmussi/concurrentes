//
// Created by tomas on 07/09/17.
//

#ifndef PRUEBAS_CONCURRENTES_SHMUSAGE_H
#define PRUEBAS_CONCURRENTES_SHMUSAGE_H


class SHMUsage {
private:
    int calcularRandom ();
public:
    void useOne();
    void useTwo();
};


#endif //PRUEBAS_CONCURRENTES_SHMUSAGE_H
