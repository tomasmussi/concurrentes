

#include <stdlib.h>
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
#include "CourtManager.h"

#include "Logger.h"

// TODO: CUIDADO CON ESTO, CUANDO SE AGREGUE UN PROCESO HAY QUE TOCAR ESTE DEFINE
#define N_WORKERS 4


int main(int argc, char* argv[]) {
    int m, k, rows, columns;
    m = 12;
    k = 4;
    rows = 2;
    columns = 3;
    if (argc == 5) {
        m = atoi(argv[1]);
        k = atoi(argv[2]);
        rows = atoi(argv[3]);
        columns = atoi(argv[4]);
    }

    MainSIGIntHandler sigint_handler;
    SignalHandler :: getInstance()->registrarHandler ( SIGINT,&sigint_handler );
    Logger::open_logger("log.txt");
    Logger::log("main", Logger::INFO, "Comienzo", Logger::get_date());

    std::string fifo2 = "/tmp/fifo2";
    std::string fifo3 = "/tmp/fifo3";

    WorkerProcess* arr[N_WORKERS] = {new PeopleRegisterWorker(),
                                     new BeachManagerWorker(fifo2),
                                     new TeamMaker(m, k, fifo2, fifo3),
                                     new CourtManager(m,k,rows, columns, fifo3) };

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
            Logger::log("main", Logger::INFO, s, Logger::get_date());
            std::cout << s << std::endl;
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
    Logger::close_logger();
    return son_process;
}
