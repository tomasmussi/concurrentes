

#include <stdlib.h>

#include "Subscriber.h"

/*
int main(int argc, char* argv[]) {
    int k, rows, columns;
    k = 7;
    rows = 2;
    columns = 3;
    if (argc == 4) {
        k = atoi(argv[1]);
        rows = atoi(argv[2]);
        columns = atoi(argv[3]);
    }

    Subscriber subscriber;
    TournamentManager manager(k, rows, columns);
    subscriber.add_manager(&manager);
    for (int i = 0; i < 10; i++) {
        subscriber.subscribe_person(i);
    }
    while (manager.do_job()) {
        subscriber.do_job();
    }
    manager.get_recorder()->list_all_matches();
    return 0;
}*/

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cerrno>
#include <cstring>
#include <sstream>


#include "MainSIGIntHandler.h"
#include "SignalHandler.h"

#include "PeopleRegisterWorker.h"
#include "BeachManagerWorker.h"
#include "TeamMaker.h"

#include "Logger.h"
#include "NewPlayerHandler.h"

// TODO: CUIDADO CON ESTO, CUANDO SE AGREGUE UN PROCESO HAY QUE TOCAR ESTE DEFINE
#define N_WORKERS 1

int main(int argc, char* argv[]) {
    Logger::open_logger("log.txt");
    Logger::log("main", Logger::INFO, "Comienzo");
    MainSIGIntHandler sigint_handler;
    SignalHandler :: getInstance()->registrarHandler ( SIGINT,&sigint_handler );
    Logger::log("main", Logger::DBG, "MainSIGIntHandler registrado");

    std::string fifo1 = "/tmp/fifo1";
    std::string fifo2 = "/tmp/fifo2";
    std::string fifo3 = "/tmp/fifo3";

    std::stringstream ss;
    ss << "Agregando handler para nuevos players en " << getpid();
    std::string s = ss.str();
    Logger::log("main", Logger::DBG, s);
    NewPlayerHandler new_player_handler(fifo1);
    SignalHandler::getInstance()->registrarHandler(SIGUSR1, &new_player_handler);
    Logger::log("main", Logger::DBG, "NewPlayerHandler registrado");

    WorkerProcess* arr[N_WORKERS] = {//new PeopleRegisterWorker(),
                                     new BeachManagerWorker(fifo1, fifo2)};

    bool is_father = true;
    int son_process = 0;
    for (int i = 0; i < N_WORKERS; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            is_father = false;
            son_process = arr[i]->loop();
            break;
        } else {
            std::stringstream ss;
            ss << "Nuevo worker " << arr[i]->prettyName() << " con pid " << pid;
            std::string s = ss.str();
            Logger::log("main", Logger::DBG, s);
            sigint_handler.add_pid_notification(pid);
        }
    }
    if (is_father) {
        // Esto es para que se abra el fifo1 para escribir
        new_player_handler.initialize();
        int collected = 0;
        while (collected < N_WORKERS) {
            int status;
            pid_t child_pid = wait(&status);
            // El proceso padre de todos llama a wait esperando que sus hijos terminen.
            // Sin embargo, tambien es el que captura el SIGINT, por lo que se despierta
            // con la captura de SIGINT y se setea errno con EINTR porque fue interrumpido mientras estaba
            // bloqueado en una system call(wait). Por eso en definitiva hay que chequear esto
            if (child_pid == -1 && errno != EINTR) {
                std::cerr << "ERROR: " << std::strerror(errno) << std::endl;
            } else if (child_pid > 0) {
                std::cout << "main:: [" << child_pid << "] termino con: " << WEXITSTATUS(status) << std::endl;
                collected++;
            }
        }
    }
    for (int i = 0; i < N_WORKERS; i++) {
        delete arr[i];
    }
    SignalHandler::destroy();
    Logger::close_logger();
    return son_process;
}
