#include <sys/wait.h>
#include <sstream>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include "CourtManager.h"
#include "../ipc/SignalHandler.h"
#include "../constants.h"

#define EMPTY 0
#define OCCUPIED 1
#define FLOODED 2

CourtManager::CourtManager(int m, int k, int rows, int columns, const std::string& fifo_read,
        const std::string& fifo_write_people, const std::string& fifo_write_matches ) :
    _m(m), _k(k), _rows(rows), _columns(columns),
    _fifo_read(fifo_read), _fifo_write_people(fifo_write_people), _fifo_write_matches(fifo_write_matches),
    _available_courts(SEM_AVAILABLE_COURTS, rows * columns), _court_state(),_tide_column(_columns), _flooded_matches() {
    for (int i = 0; i < _rows; i++) {
        for (int j = 0; j < _columns; j++) {
            _court_state[i][j] = EMPTY;
            _court_pid[i][j] = 0;
        }
    }
}

CourtManager::~CourtManager() {
}

int CourtManager::do_work() {
    Logger::log(prettyName(), Logger::DEBUG, "Esperando que se desocupe cancha", Logger::get_date());
    int status = _available_courts.p(); // Resto una cancha libre
    while (graceQuit() == 0 && status == -1 &&  errno == EINTR) {
        // Fallo la system call por interrupcion, pero debo seguir trabajando
        status = _available_courts.p();
    }
    Logger::log(prettyName(), Logger::DEBUG, "Cancha desocupada", Logger::get_date());
    // TODO: Cambiar para que se encolen teams en el fifo como si recien ingresaran
    if (_flooded_matches.size() > 0) {
        // Despacho los que se suspendieron por inundacion
        Match flooded_match = _flooded_matches.front();
        _flooded_matches.pop_front();
        dispatch_match(flooded_match.team1(), flooded_match.team2());
    } else {
        // Leo nuevos partidos del FIFO
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
    }
    return 0;
}

bool CourtManager::occupy_court(pid_t pid) {
    bool occupied = false;
    int i = 0;
    while (!occupied && i < _rows) {
        int j = 0;
        while (!occupied && j < _columns) {
            if (_court_state[i][j] == EMPTY) {
                _court_state[i][j] = OCCUPIED;
                _court_pid[i][j] = pid;
                occupied = true;
                std::stringstream ss;
                ss << "OCUPO CANCHA en [" << i << "][" << j << "] = " << pid;
                Logger::log(prettyName(), Logger::INFO, ss.str(), Logger::get_date());
            }
            j++;
        }
        i++;
    }
    return occupied;
}

void CourtManager::dispatch_match(const Team& team1, const Team& team2) {
    Match match(team1, team2);
    // Esto es para crear el MatchProcess solo en el hijo, donde tiene sentido
    pid_t father_pid = getpid();
    std::string match_between = "entre " + team1.to_string() + " y " + team2.to_string();
    pid_t pid = fork();
    if (pid > 0) {
        bool occupied = occupy_court(pid);
        if (!occupied) {
            Logger::log(prettyName(), Logger::WARNING, "No se pudo ocupar cancha", Logger::get_date());
        }
        // Proceso padre
        _matches[pid] = match;
        std::stringstream ss;
        ss << "Despachado MatchProcess[" << pid << "] " << match_between;
        Logger::log(prettyName(), Logger::INFO, ss.str(), Logger::get_date());
    } else {
        MatchProcess match_process(father_pid);

        // Proceso hijo, dispatch crea una SHM y sale con exit de la ejecucion de todo
        Logger::log(match_process.prettyName(), Logger::INFO, "Arrancando partido " + match_between, Logger::get_date());

        match_process.run_match();
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

    SignalHandler::getInstance()->registrarHandler(SIGUSR2, this);
    SignalHandler::getInstance()->registrarHandler(SIGUNUSED, this);
    SignalHandler::getInstance()->registrarHandler(SIGCHLD, this);
    Logger::log(prettyName(), Logger::INFO, "Inicializado", Logger::get_date());
}

void CourtManager::finalize() {
    Logger::log(prettyName(), Logger::DEBUG, "Finalizando", Logger::get_date());
    kill_matches();
    Logger::log(prettyName(), Logger::DEBUG, "Partidos cancelados", Logger::get_date());
    _fifo_read.cerrar();
    _fifo_write_people.cerrar();
    _fifo_write_matches.cerrar();
    Logger::log(prettyName(), Logger::DEBUG, "Fifos cerrados", Logger::get_date());
    _available_courts.remove();
    Logger::log(prettyName(), Logger::DEBUG, "Semaforo removido", Logger::get_date());
    SignalHandler::destroy();
    Logger::log(prettyName(), Logger::INFO, "Finalizado", Logger::get_date());
}

std::string CourtManager::prettyName() {
    return "Court Manager";
}

int CourtManager::handleSignal(int signum) {
    Logger::log(prettyName(), Logger::DEBUG, "Handling signal", Logger::get_date());
    switch (signum) {
        case SIGUSR2:
            tide_rise(signum);
            break;
        case SIGUNUSED:
            tide_decrease(signum);
            break;
        case SIGCHLD:
            handle_matches(signum);
            break;
    }
    return 0;
}

// Se llama en caso de recibir un SIGINT
void CourtManager::kill_matches() {
    Logger::log(prettyName(), Logger::INFO, "Recibido SIGINT, enviando señal a los MatchProcess en curso", Logger::get_date());
    for (std::map<pid_t,Match>::iterator it = _matches.begin(); it != _matches.end(); it++ ){
        std::stringstream ss;
        ss << "Notificando SIGINT a MatchProcess[" << it->first << "] ";
        Logger::log(prettyName(), Logger::INFO, ss.str(), Logger::get_date());
        kill(it->first, SIGINT);
    }
}

void CourtManager::tide_rise(int ) {
    Logger::log(prettyName(), Logger::INFO, "Subiendo marea", Logger::get_date());
    if (_tide_column == 1) {
        // No puede subir mas la marea
        Logger::log(prettyName(), Logger::WARNING, "La marea no puede aumentar mas", Logger::get_date());
        return;
    }
    _tide_column--;
    std::stringstream ss;
    ss << "Ahora la columna de agua esta en: " << _tide_column;
    Logger::log(prettyName(), Logger::DEBUG, ss.str(), Logger::get_date());
    std::list<pid_t> matches_to_kill;
    for (int row = 0; row < _rows; row++) {
        pid_t match_pid = _court_pid[row][_tide_column];
        if (_court_state[row][_tide_column] == OCCUPIED) {
            // Cuando se despacho el partido se hizo "p"
            matches_to_kill.push_back(match_pid);
        } else if (_court_state[row][_tide_column] == EMPTY) {
            // Si esta libre, hay que decrementar el semaforo para indicar que hay una cancha menos disponible
            _available_courts.p();
        }
        std::stringstream ss;
        ss << "Inundando cancha [" << row << "][" << _tide_column << "] = " << match_pid;
        Logger::log(prettyName(), Logger::INFO, ss.str(), Logger::get_date());
        _court_state[row][_tide_column] = FLOODED;
        _court_pid[row][_tide_column] = 0;
    }

    Logger::log(prettyName(), Logger::DEBUG, "Matando MatchesProcess que estaban en canchas inundadas", Logger::get_date());
    for (std::list<pid_t>::iterator it = matches_to_kill.begin(); it != matches_to_kill.end(); ++it) {
        kill((*it), SIGUSR1);
        // No llamo al wait ya que, como mande un SIGUSR1, va a terminar mandando un SIGCHLD que va a handlearse más tarde haciendo el wait
        std::stringstream ss;
        ss << "MatchProcess " << *it << " finalizado por inundacion";
        Logger::log(prettyName(), Logger::INFO, ss.str(), Logger::get_date());
    }
}

void CourtManager::tide_decrease(int ) {
    Logger::log(prettyName(), Logger::INFO, "Bajando marea", Logger::get_date());
    if (_tide_column == _columns) {
        // No puede bajar mas la marea;
        Logger::log(prettyName(), Logger::WARNING, "La marea no puede bajar mas: ya estan todas las canchas disponibles", Logger::get_date());
        return;
    }
    for (int row = 0; row < _rows; row++) {
        if (_court_state[row][_tide_column] == FLOODED) {
            _court_state[row][_tide_column] = EMPTY;
        } else {
            std::stringstream ss;
            ss << " Cancha [" << row << "][" << _tide_column << "] esta en estado ";
            if (_court_state[row][_tide_column] == EMPTY){
                ss << "EMPTY";
            } else {
                ss << "FLOODED";
            }
            Logger::log(prettyName(), Logger::INFO, ss.str(), Logger::get_date());
        }
    }
    // Ahora tengo "_columnas" de canchas mas disponibles para usar
    _available_courts.v(_columns);
    _tide_column++;
    std::stringstream ss;
    ss << "Ahora la columna de agua esta en: " << _tide_column;
    Logger::log(prettyName(), Logger::DEBUG, ss.str(), Logger::get_date());
}

void CourtManager::handle_matches(int signum) {
    Logger::log(prettyName(), Logger::INFO, "Handleando partido", Logger::get_date());
    int saved_errno = errno;
    int status;
    // Al recibir SIGCHLD, ejecuto waitpid tantas veces como sea necesario para recolectar todos los MatchProcess que hayan finalizado
    // TODO: Esto puede ser que se quede esperando un proceso que va a terminar mas tarde, mientras podría estar despachando otros procesos?
    pid_t pid = waitpid((pid_t)(-1), &status, 0);
    while (pid > 0) {
        std::stringstream ss;
        ss << "Procesando MatchProcess: " << pid;
        Logger::log(prettyName(), Logger::INFO, ss.str(), Logger::get_date());
        process_finished_match(pid,status);
        pid = waitpid((pid_t)(-1), &status, 0);
    }
    if (pid == -1 ) {
        if (errno == ECHILD) {
            Logger::log(prettyName(), Logger::INFO, "No hay más partidos para procesar", Logger::get_date());
        } else if (errno = EINTR) {
            Logger::log(prettyName(), Logger::WARNING, "El wait fue interrumpido: " + std::string(std::strerror(errno)), Logger::get_date());
        }
    }
    errno = saved_errno;
}

void CourtManager::process_finished_match(pid_t match_pid, int status) {
    Match match = _matches[match_pid];
    match.set_match_status(WEXITSTATUS(status));
    Logger::log(prettyName(), Logger::DEBUG, match.to_string(), Logger::get_date());
    //_matches[match_pid] = match;
    //Finalizado el partido, lo borro para mantener los MatchProcess activos
    _matches.erase(match_pid);
    if (match.flooded()) {
        // La cancha se inundo
        std::stringstream ss;
        ss << "MatchProcess [" << match_pid << "] inundado";
        Logger::log(prettyName(), Logger::INFO, ss.str(), Logger::get_date());
        // TODO ACA DEBERIA REESCRIBIR LOS EQUIPOS EN EL FIFO DE LECTURA DEL COURTMANAGER!!!!
        _flooded_matches.push_back(match);
        return;
    }
    // De aca en adelante, el partido termino
    std::stringstream ss;
    ss << "MatchProcess [" << match_pid << "] finalizado";
    Logger::log(prettyName(), Logger::INFO, ss.str(), Logger::get_date());
    if (!match.finished()) {
        // El partido fue interrumpido por SIGINT, por lo que no me interesa volver a mandar las cosas
        Logger::log(prettyName(), Logger::INFO, "El partido fue interrumpido por SIGINT", Logger::get_date());
        return;
    }
    // El partido termino exitosamente, por lo que hay que escribir el resultado
    Logger::log(prettyName(), Logger::DEBUG, "Enviando resultado del partido", Logger::get_date());
    _fifo_write_matches.escribir(static_cast<void*>(&match), sizeof(Match));

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

    bool freed = free_court(match_pid);
    if (!freed) {
        Logger::log(prettyName(), Logger::WARNING, "No se pudo liberar cancha", Logger::get_date());
    }

    int sem_status = _available_courts.v(); // Sumo a la cantidad de canchas disponible
    while (graceQuit() == 0 && sem_status == -1 && errno == EINTR) {
        // Fallo la system call por interrupt
        sem_status = _available_courts.v();
    }
}

bool CourtManager::free_court(pid_t pid) {
    int i = 0;
    bool freed = false;
    while (!freed && i < _rows) {
        int j = 0;
        while (!freed && j < _columns) {
            if (_court_pid[i][j] == pid) {
                _court_state[i][j] = EMPTY;
                _court_pid[i][j] = 0;
                freed = true;
                std::stringstream ss;
                ss << "Libero cancha en [" << i << "][" << j << "] = " << pid;
                Logger::log(prettyName(), Logger::INFO, ss.str(), Logger::get_date());
            }
            j++;
        }
        i++;
    }
    return freed;
}
