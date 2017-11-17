//
// Created by tomas on 17/11/17.
//

#ifndef TP2_MENSAJES_H
#define TP2_MENSAJES_H

#define DEBUG 1

#define MSG_ARCHIVO "/bin/bash"
#define CHAR_CLIENTE_SERVIDOR 'a'

#define	PETICION	1
#define	RESPUESTA	2
#define TEXTO_SIZE	255


typedef struct mensaje {
    long mtype;
    int id;
    char texto[TEXTO_SIZE];
} mensaje;


#endif //TP2_MENSAJES_H
