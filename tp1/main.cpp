#include <unistd.h>
#include <sys/wait.h>
#include <cerrno>
#include <cstring>
#include <ctime>
#include <sstream>
#include <cstdlib>
#include <sstream>

#include "handlers/MainSIGIntHandler.h"
#include "ipc/SignalHandler.h"
#include "ipc/Semaphore.h"
#include "handlers/NewPlayerHandler.h"

#include "process/BeachManagerWorker.h"
#include "process/TeamMaker.h"
#include "process/CourtManager.h"
#include "process/ResultsReporter.h"
#include "process/Timer.h"

#include "constants.h"

// TODO: CUIDADO CON ESTO, CUANDO SE AGREGUE UN PROCESO HAY QUE TOCAR ESTE DEFINE
#define N_WORKERS 5

int parse_int(char* arg, int default_value, std::string field) {
    std::istringstream ss(arg);
    int x;
    std::stringstream s;
    if (!(ss >> x)) {
        s << "Valor invalido '" << arg << "' para '" << field << "'. Tomando valor por defecto de " << default_value;
        Logger::log("main", Logger::WARNING, s.str(), Logger::get_date());
        x = default_value;
    } else if (x < 0) {
        s << "'" << field << "' debe ser mayor que 0. Tomando valor por defecto de " << default_value;
        Logger::log("main", Logger::WARNING, s.str(), Logger::get_date());
        x = default_value;
    } else if (x == 0 && field != "Nivel de log (log_level)") {
        s << "Tomando valor por defecto de " << default_value << " para '" << field << "'";
        Logger::log("main", Logger::INFO, s.str(), Logger::get_date());
        x = default_value;
    }
    return x;
}

int main(int argc, char* argv[]) {
    Logger::open_logger(LOG_NAME);
    int m, k, rows, columns, log_level;
    if (argc == 6) {
        m = parse_int(argv[1], M, "Cantidad de jugadores simultaneos (m)");
        if (m < MIN_PEOPLE) {
            std::stringstream ss;
            ss << "El valor de 'm' debe ser mayor o igual que " << MIN_PEOPLE << ". Tomando valor por defecto de " << M;
            Logger::log("main", Logger::WARNING, ss.str(), Logger::get_date());
            m = M;
        }
        k = parse_int(argv[2], K, "Cantidad maxima de partidos (k)");
        rows = parse_int(argv[3], ROWS, "Filas de canchas (rows)");
        columns = parse_int(argv[4], COLUMNS, "Columnas de canchas (columns)");
        log_level = parse_int(argv[5], LOG_LEVEL, "Nivel de log (log_level)");
        if (log_level > Logger::ERROR) {
            std::stringstream ss;
            ss << "El valor de 'log_level' debe ser menor o igual que " << Logger::ERROR << ". Tomando valor por defecto de " << LOG_LEVEL;
            Logger::log("main", Logger::WARNING, ss.str(), Logger::get_date());
            log_level = LOG_LEVEL;
        }
    } else {
        Logger::log("main", Logger::WARNING, "La cantidad de argumentos debe ser igual a 5. Tomando todos los valores por defecto...", Logger::get_date());
        m = M;
        k = K;
        rows = ROWS;
        columns = COLUMNS;
        log_level = LOG_LEVEL;
    }
    Logger::set_log_level(log_level);

    MainSIGIntHandler sigint_handler;
    SignalHandler :: getInstance()->registrarHandler ( SIGINT,&sigint_handler );
    Logger::log("main", Logger::INFO, "MainSIGIntHandler registrado", Logger::get_date());

    std::string fifo1 = FIFO1; // people (NewPlayerHandler -> BeachManagerWorker)
    std::string fifo2 = FIFO2; // people (BeachManagerWorker/CourtManager -> TeamMaker)
    std::string fifo3 = FIFO3; // teams (TeamMaker -> CourtManager)
    std::string fifo4 = FIFO4; // matches (CourtManager -> ResultsReporter)

    Semaphore players_playing(SEM_PLAYERS_PLAYING, m);
    Semaphore tournament_started(SEM_TOURNAMENT_STARTED, 0);

    WorkerProcess* arr[N_WORKERS] = {new Timer(tournament_started),
                                     new BeachManagerWorker(fifo1, fifo2, players_playing, tournament_started),
                                     new TeamMaker(k, fifo2, fifo3, players_playing),
                                     new CourtManager(m, k, rows, columns, fifo3, fifo2, fifo4),
                                     new ResultsReporter(fifo4)};

    // Seteo la semilla del random para el programa
    std::srand((unsigned) std::time(NULL));

    bool is_father = true;
    int son_process = 0;
    int timer_pid = 0;
    for (int i = 0; i < N_WORKERS; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            is_father = false;
            if (i == 3) ((CourtManager*) arr[i])->setTimerPid(timer_pid);
            son_process = arr[i]->loop();
            break;
        } else {
            std::stringstream ss;
            ss << "Nuevo worker " << arr[i]->prettyName() << " con pid " << pid;
            Logger::log("main", Logger::INFO, ss.str(), Logger::get_date());
            sigint_handler.add_pid_notification(pid);
            // TODO: Cuidado si se cambia el orden de los workers!
            if (i == 0) timer_pid = pid;
        }
    }

    if (is_father) {
        // Esto es para que se abra el fifo1 para escribir
        std::stringstream ss;
        ss << "Agregando handler para nuevos jugadores en " << getpid();
        std::string s = ss.str();
        Logger::log("main", Logger::INFO, s, Logger::get_date());
        NewPlayerHandler new_player_handler(fifo1);
        SignalHandler::getInstance()->registrarHandler(SIGUSR1, &new_player_handler);
        Logger::log("main", Logger::DEBUG, "NewPlayerHandler registrado", Logger::get_date());
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
    Logger::close_logger(is_father);
    return son_process;
}
