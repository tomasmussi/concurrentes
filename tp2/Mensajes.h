#ifndef TP2_MENSAJES_H
#define TP2_MENSAJES_H

#define MSG_ARCHIVO "/bin/bash"
#define CHAR_CLIENTE_SERVIDOR 'a'
#define CHAR_SERVIDOR_SERVICIOS 'b'

#define	TIEMPO 1
#define	MONEDA 2

#define READ_MENSAJES -2

#define TEXTO_SIZE	255

typedef struct tiempo {
    double temperatura;
    int presion;
    double humedad;
} tiempo;

typedef struct mensaje {
    long mtype;
    int id;
    char texto[TEXTO_SIZE];
    bool write;
    tiempo tiemp;
    double cambio;
} mensaje;

#endif //TP2_MENSAJES_H
