#include <sys/wait.h>
#include <sstream>
#include <cstdlib>

#include "CourtManager.h"
#include "../ipc/SignalHandler.h"
#include "../constants.h"

CourtManager::CourtManager(int m, int k, int rows, int columns, const std::string& fifo_read,
        const std::string& fifo_write_people, const std::string& fifo_write_matches ) :

    _m(m), _k(k), _rows(rows), _columns(columns), _fifo_read(fifo_read), _fifo_write_people(fifo_write_people), _fifo_write_matches(fifo_write_matches),
    _lock_matches(SHM_MATCHES_LOCK), _shm_matches(),
    _available_courts(SEM_AVAILABLE_COURTS, rows * columns) {


}

CourtManager::~CourtManager() {
}

int CourtManager::do_work() {


    Logger::log(prettyName(), Logger::DEBUG, "Esperando que se desocupe cancha", Logger::get_date());
    _available_courts.p(); // Resto una cancha libre
    Logger::log(prettyName(), Logger::DEBUG, "Cancha desocupada", Logger::get_date());

    Team team1;
    Team team2;
    while (graceQuit() == 0 && !team1.valid()) {
        _fifo_read.leer(static_cast<void*>(&team1), sizeof(Team));
    }
    if (team1.valid()) {
        Logger::log(prettyName(), Logger::INFO, "Recibido equipo 1: " + team1.to_string(), Logger::get_date());
        while (graceQuit() == 0 && !team2.valid()) {
            _fifo_read.leer(static_cast<void*>(&team2), sizeof(Team));
        }
        if (team2.valid()) {
            Logger::log(prettyName(), Logger::INFO, "Recibido equipo 2: " + team2.to_string(), Logger::get_date());
            dispatch_match(team1, team2);
        }
    }
    return 0;
}

void CourtManager::dispatch_match(const Team& team1, const Team& team2) {
    Match match(team1, team2);
    std::string timestamp = Logger::get_date();
    // Esto es para crear el MatchProcess solo en el hijo, donde tiene sentido
    pid_t father_pid = getpid();
    std::string match_between = "entre " + team1.to_string() + " y " + team2.to_string();
    pid_t pid = fork();
    if (pid > 0) {
        // Proceso padre
        _matches[pid] = match;
        std::stringstream ss;
        ss << "Despachado MatchProcess[" << pid << "] " << match_between;
        Logger::log(prettyName(), Logger::INFO, ss.str(), timestamp);
    } else {
        MatchProcess match_process(father_pid);
        // Proceso hijo, dispatch crea una SHM y sale con exit de la ejecucion de todo
        Logger::log(match_process.prettyName(), Logger::INFO, "Arrancando partido " + match_between, Logger::get_date());
        match_process.dispatch_match();
        int match_result = match_process.get_match_result();
        // Llamo al finalize porque al salir del proceso con un exit, no se ejecuta el destructor del MatchProcess
        match_process.finalize();
        exit(match_result);
    }
}

void CourtManager::initialize() {
    Logger::log(prettyName(), Logger::DEBUG, "Inicializando", Logger::get_date());
    _fifo_read.abrir();
    Logger::log(prettyName(), Logger::DEBUG, "Fifo de lectura de equipos de TeamMaker abierto", Logger::get_date());
    _fifo_write_people.abrir();
    Logger::log(prettyName(), Logger::DEBUG, "Fifo de envio de personas a TeamMaker abierto", Logger::get_date());

    _fifo_write_matches.abrir();
    Logger::log(prettyName(), Logger::DEBUG, "Fifo de envio de partidos a ResultsReporter abierto", Logger::get_date());

    try {
        initialize_shm_couples();
        Logger::log(prettyName(), Logger::DEBUG, "Shared Memory Pareja Personas inicializada", Logger::get_date());
    } catch (const std::string& error) {
        Logger::log(prettyName(), Logger::ERROR, error, Logger::get_date());
    }
    try {
        initialize_shm_mapper();
        Logger::log(prettyName(), Logger::DEBUG, "Shared Memory Mapper inicializada", Logger::get_date());
    } catch (const std::string& error) {
        Logger::log(prettyName(), Logger::ERROR, error, Logger::get_date());
    }
    _shm_matches.crear(SHM_MATCHES, SHM_MATCHES_CHAR);


    _lock_matches.lock();
    _shm_matches.escribir(0);
    _lock_matches.release();
    SignalHandler::getInstance()->registrarHandler(SIGUSR1, this);
    Logger::log(prettyName(), Logger::INFO, "Inicializado", Logger::get_date());
}

void CourtManager::finalize() {
    Logger::log(prettyName(), Logger::DEBUG, "Finalizando", Logger::get_date());
    _fifo_read.cerrar();
    _fifo_write_people.cerrar();
    _fifo_write_matches.cerrar();
    Logger::log(prettyName(), Logger::DEBUG, "Fifos cerrados", Logger::get_date());
    _shm_matches.liberar();
    Logger::log(prettyName(), Logger::DEBUG, "SHM matches destruida", Logger::get_date());
    SignalHandler::destroy();
    Logger::log(prettyName(), Logger::INFO, "Finalizado", Logger::get_date());
}

std::string CourtManager::prettyName() {
    return "Court Manager";
}

int CourtManager::handleSignal(int signum) {
    Logger::log(prettyName(), Logger::DEBUG, "Handling signal", Logger::get_date());
    if (signum != SIGUSR1) {
        Logger::log(prettyName(), Logger::ERROR, "Recibi senial distinta a SIGUSR1", Logger::get_date());
        return 1;
    }
    Logger::log(prettyName(), Logger::DEBUG, "Tomando lock de matches", Logger::get_date());
    try {
        _lock_matches.lock();
        int matches_to_process = _shm_matches.leer();
        std::stringstream s;
        s << "Procesando " << matches_to_process << " partidos";
        Logger::log(prettyName(), Logger::INFO, s.str(), Logger::get_date());
        for (int i = 0; i < matches_to_process; i++) {
            process_finished_match();
        }
        _shm_matches.escribir(0);
        _lock_matches.release();
    } catch (const std::string& excp) {
        Logger::log(prettyName(), Logger::DEBUG, "Exception: " + excp, Logger::get_date());
    }

    Logger::log(prettyName(), Logger::DEBUG, "Lock de matches liberado", Logger::get_date());
    return 0;
}

void CourtManager::process_finished_match() {
    int status;
    pid_t match_pid = wait(&status);
    Match match = _matches[match_pid];
    match.set_match_status(WEXITSTATUS(status));
    _matches[match_pid] = match;
    if (match.finished()) {
        std::stringstream ss;
        ss << "MatchProcess [" << match_pid << "] " << "finalizado";
        //ss << "finalizado " << match.to_string();
        Logger::log(prettyName(), Logger::INFO, ss.str(), Logger::get_date());
        // El partido termino, por lo que hay que escribir el resultado
        _fifo_write_matches.escribir(static_cast<void*>(&match), sizeof(Match));

    } else {
        std::stringstream ss;
        ss << "MatchProcess [" << match_pid << "] ";
        ss << "inundado " << match.to_string();
        Logger::log(prettyName(), Logger::INFO, ss.str(), Logger::get_date());
    }
    // Envio a Team Maker T1-P1; T2-P2; T1-P2; T2-P1
    Person p;
    p = match.team1().get_person1();
    _fifo_write_people.escribir(static_cast<void*>(&p), sizeof(Person));
    Logger::log(prettyName(), Logger::INFO, "Enviada persona " + p.id() + " a TeamMaker", Logger::get_date());

    p = match.team2().get_person2();
    _fifo_write_people.escribir(static_cast<void*>(&p), sizeof(Person));
    Logger::log(prettyName(), Logger::INFO, "Enviada persona " + p.id() + " a TeamMaker", Logger::get_date());

    p = match.team1().get_person2();
    _fifo_write_people.escribir(static_cast<void*>(&p), sizeof(Person));
    Logger::log(prettyName(), Logger::INFO, "Enviada persona " + p.id() + " a TeamMaker", Logger::get_date());

    p = match.team2().get_person1();
    _fifo_write_people.escribir(static_cast<void*>(&p), sizeof(Person));
    Logger::log(prettyName(), Logger::INFO, "Enviada persona " + p.id() + " a TeamMaker", Logger::get_date());

    _available_courts.v(); // Sumo a la cantidad de canchas disponible
}
