#include "MatchProcess.h"
#include "../constants.h"
#include "../ipc/SignalHandler.h"
#include <stdlib.h>
#include <cstdlib>
#include <signal.h>

MatchProcess::MatchProcess(pid_t parent_process_id) : _father_id(parent_process_id),
    _probability(0.5), _score_team1(0), _score_team2(0),
    _lock_matches(SHM_MATCHES_LOCK), _shm_matches(NULL) {
        SignalHandler::getInstance()->registrarHandler(SIGINT, this);
}

MatchProcess::~MatchProcess() {
    finalize();
}

int MatchProcess::handleSignal(int signum) {
    if (signum != SIGINT) {
        Logger::log(prettyName(), Logger::ERROR, "Recibi senial distinta a SIGINT", Logger::get_date());
        return -1;
    }
    Logger::log(prettyName(), Logger::DEBUG, "Finalizando debido a SIGINT", Logger::get_date());
    finalize();
    exit(7); //Un número distinto a todos los resultados posibles del partido
}

void MatchProcess::finalize() {
    if (_shm_matches != NULL) {
        Logger::log(prettyName(), Logger::INFO, "Destruyendo SHM de matches", Logger::get_date());
        try {
            _shm_matches->liberar();
        } catch (const std::string& excp) {
            Logger::log(prettyName(), Logger::DEBUG, "SHM LIBERAR ERROR ", Logger::get_date());
        }
        delete(_shm_matches);
        _shm_matches = NULL;
    } else {
        Logger::log(prettyName(), Logger::WARNING, "Al eliminar: No toco shm porque es null", Logger::get_date());
    }
}

/**
 * Al llamar a esta funcion, se pasa a desarrollar un partido en un proceso aparte
 * Si no se llama a esta funcion, se puede utilizar el objeto como contenedor de
 * informacion
 * */
void MatchProcess::dispatch_match() {
    Logger::log(prettyName(), Logger::DEBUG, "Creando memoria compartida de matches", Logger::get_date());
    // TODO: No seria mejor si _shm_matches es una variable estatica en vez de dinamica?
    _shm_matches = new MemoriaCompartida<int>;
    _shm_matches->crear(SHM_MATCHES, SHM_MATCHES_CHAR);

    this->run_match();

    std::string timestamp = Logger::get_date();
    Logger::log(prettyName(), Logger::INFO, "Senializado al CourtManager sobre el fin del partido", timestamp);
    signal_court_manager();
    Logger::log(prettyName(), Logger::DEBUG, "Ahora deberia venir el dt de SHM", timestamp);
}

// Simular el partido y dar a un equipo como ganador
void MatchProcess::run_match() {
    srand(static_cast<unsigned int>(time(0)+getpid())); //getpid evita que dos partidos que arrancan al mismo tiempo, tengan el mismo resultado
    int play_time = rand() % MAX_MATCH_DURATION;
    // Simulo que el partido dura un tiempo aleatorio entre 0 y 9 segundos
    sleep(play_time);
    int who_wins = rand() % 100;
    if (MATCH_PROBABILITY >= who_wins) {
        // team1 wins
        set_scores(_score_team1, _score_team2);
    } else {
        // team2 wins
        set_scores(_score_team2, _score_team1);
    }
    std::stringstream ss;
    ss << "El partido duro " << play_time << " segundos y termino: " << _score_team1 << " a " << _score_team2;
    Logger::log(prettyName(), Logger::INFO, ss.str(), Logger::get_date());
}

void MatchProcess::set_scores(int& score_winner, int& score_loser) {
    score_winner = 3;
    int random = rand() % 100;
    if (random < 33) {
        score_loser = 0;
    }
    else if (random < 66) {
        score_loser = 1;
    }
    else {
        score_loser = 2;
    }
}

void MatchProcess::signal_court_manager() {
    Logger::log(prettyName(), Logger::DEBUG, "Tomando lock de matches", Logger::get_date());
    _lock_matches.lock();
    _shm_matches->escribir(_shm_matches->leer() + 1);
    _lock_matches.release();
    Logger::log(prettyName(), Logger::DEBUG, "Lock de matches liberado", Logger::get_date());
    kill(_father_id, SIGUSR1);
}

/**
 * Recordar:
 * 0 : Termino 3-0
 * 1 : Termino 3-1
 * 2 : Termino 3-2
 * 3 : Termino 2-3
 * 4 : Termino 1-3
 * 5 : Termino 0-3
 * 6 : No termino
 * */
int MatchProcess::get_match_result() {
    if (_score_team1 == 3 && _score_team2 == 0) {
        return 0;
    } else if (_score_team1 == 3 && _score_team2 == 1) {
        return 1;
    } else if (_score_team1 == 3 && _score_team2 == 2) {
        return 2;
    } else if (_score_team1 == 2 && _score_team2 == 3) {
        return 3;
    } else if (_score_team1 == 1 && _score_team2 == 3) {
        return 4;
    } else if (_score_team1 == 0 && _score_team2 == 3) {
        return 5;
    }
    return 6;
}

std::string MatchProcess::prettyName() {
    std::stringstream ss;
    ss << "Match Process (" << getpid() << ")";
    return ss.str();
}
