#ifndef TP1_CONSTANTS_H
#define TP1_CONSTANTS_H

// Valores default
#define M 12
#define K 9
#define ROWS 2
#define COLUMNS 3
#define LOG_LEVEL 0

// Logger
#define LOG_NAME "log.txt"
#define LOGGER_LOCK "/tmp/logger_lock"

// SHMs
#define SHM_GONE_PLAYERS_LOCK "/tmp/shm_gone_users"
#define SHM_GONE_PLAYERS "/bin/echo"

// Semaforos
#define SEM_AVAILABLE_COURTS "/bin/bash"
#define SEM_PLAYERS_PLAYING "/bin/cat"
#define SEM_TOURNAMENT_STARTED "/bin/grep"

// Fifos
#define FIFO1 "/tmp/fifo1"
#define FIFO2 "/tmp/fifo2"
#define FIFO3 "/tmp/fifo3"
#define FIFO4 "/tmp/fifo4"

// Probabilidades
#define MATCH_PROBABILITY 50 // Probabilidad de que gane uno u otro equipo
#define LEAVE_PROBABILITY 10 // Probabilidad de que un jugador decida irse voluntariamente
#define GONE_PLAYER_PROBABILITY 50 // Probabilidad de tomar un jugador que se fue para que vuelva a ingresar

// Otros
#define MIN_PEOPLE 10 // Cantidad minima de personas para arrancar el torneo
#define MAX_MATCH_DURATION 5 // Duracion maxima de un partido (Es decir, sera entre 1 y 5)
#define RESULTS_TABLE_FREQUENCY 5 // Cada cuantos partidos imprime la tabla de resultados
#define MAX_GONE_PLAYERS 128 // Cantidad maxima de jugadores que se fueron (sin procesar) que se pueden guardar. Este numero no debe ser mayor a 256
#define MAX_WAIT_TIME 20 // Si no se despacha un partido en este tiempo, finaliza el torneo

#endif //TP1_CONSTANTS_H
