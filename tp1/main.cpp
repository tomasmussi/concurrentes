

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


#include "MainSIGIntHandler.h"
#include "SignalHandler.h"

#include "PeopleRegisterWorker.h"
#include "BeachManagerWorker.h"


// TODO: CUIDADO CON ESTO, CUANDO SE AGREGUE UN PROCESO HAY QUE TOCAR ESTE DEFINE
#define N_WORKERS 2

int main(int argc, char* argv[]) {
    MainSIGIntHandler sigint_handler;
    SignalHandler :: getInstance()->registrarHandler ( SIGINT,&sigint_handler );
    WorkerProcess* arr[N_WORKERS] = {new BeachManagerWorker(),new PeopleRegisterWorker(),};

    bool is_father = true;
    int son_process = 0;
    for (int i = 0; i < N_WORKERS; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            is_father = false;
            son_process = arr[i]->loop();
            break;
        } else {
            sigint_handler.add_pid_notification(pid);
        }
    }
    if (is_father) {
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
    return son_process;
}
