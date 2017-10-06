#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cerrno>
#include <cstring>
#include <sstream>

#include "handlers/MainSIGIntHandler.h"
#include "ipc/SignalHandler.h"
#include "handlers/NewPlayerHandler.h"

#include "process/BeachManagerWorker.h"
#include "process/TeamMaker.h"
#include "process/CourtManager.h"

// TODO: CUIDADO CON ESTO, CUANDO SE AGREGUE UN PROCESO HAY QUE TOCAR ESTE DEFINE
#define N_WORKERS 3

int main(int argc, char* argv[]) {
    Logger::open_logger("log.txt");
    Logger::log("main", Logger::INFO, "Comienzo", Logger::get_date());
    int m, k, rows, columns;
    // TODO: m debería ser siempre mayor a MIN_PEOPLE (10), ya que sino no puede empezar el torneo
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
    Logger::log("main", Logger::DEBUG, "MainSIGIntHandler registrado", Logger::get_date());

    std::string fifo1 = "/tmp/fifo1";
    std::string fifo2 = "/tmp/fifo2";
    std::string fifo3 = "/tmp/fifo3";

    std::stringstream ss;
    ss << "Agregando handler para nuevos players en " << getpid();
    std::string s = ss.str();
    Logger::log("main", Logger::DEBUG, s, Logger::get_date());
    NewPlayerHandler new_player_handler(fifo1);
    SignalHandler::getInstance()->registrarHandler(SIGUSR1, &new_player_handler);
    Logger::log("main", Logger::DEBUG, "NewPlayerHandler registrado", Logger::get_date());

    WorkerProcess* arr[N_WORKERS] = {new BeachManagerWorker(m, fifo1, fifo2),
                                     new TeamMaker(m, k, fifo2, fifo3),
                                     new CourtManager(m, k, rows, columns, fifo3, fifo2) };

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
                Logger::log("main", Logger::ERROR, std::strerror(errno), Logger::get_date());
            } else if (child_pid > 0) {
                std::stringstream ss;
                ss << "[" << child_pid << "] termino con: " << WEXITSTATUS(status);
                Logger::log("main", Logger::INFO, ss.str(), Logger::get_date());
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
