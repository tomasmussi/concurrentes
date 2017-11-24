#ifndef TP2_MENSAJES_H
#define TP2_MENSAJES_H

#define MSG_ARCHIVO "/bin/bash"
#define CHAR_CLIENTE_SERVIDOR 'a'
#define CHAR_SERVIDOR_SERVICIOS 'b'

#define	TIEMPO 1
#define	MONEDA 2
#define TEXTO_SIZE	255

typedef struct mensajeCS { // Cliente <-> Servidor
    long mtype; // 1, 2, idCliente o idCliente + 1
    int id; // Identificador del Cliente
    int tipo; // MONEDA o TIEMPO
    bool admin; // Flag administrador (read or write)
    char texto[TEXTO_SIZE]; //Request o Respuesta
} mensajeCS;

typedef struct mensajeSS { // Servidor <-> Servicio
    long mtype; //MONEDA, TIEMPO o Identificador Cliente
    int id; // Identificador del cliente (donde se espera la respuesta)
    bool admin; //Flag administrador
    char texto[TEXTO_SIZE]; //Request o Respuesta
} mensajeSS;




#endif //TP2_MENSAJES_H
