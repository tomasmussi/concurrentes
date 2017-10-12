#include "MatchProcess.h"
#include "../constants.h"
#include "../ipc/SignalHandler.h"
#include <stdlib.h>
#include <cstdlib>
#include <signal.h>
#include <sstream>

MatchProcess::MatchProcess(pid_t parent_process_id) : _father_id(parent_process_id),
    _probability(0.5), _score_team1(0), _score_team2(0) {
        SignalHandler::getInstance()->registrarHandler(SIGINT, this);
        SignalHandler::getInstance()->registrarHandler(SIGUSR1, this);
}

MatchProcess::~MatchProcess() {
    finalize();
}

int MatchProcess::handleSignal(int signum) {
    int exit_status = -1;
    if (signum == SIGINT) {
        Logger::log(prettyName(), Logger::DEBUG, "Finalizando debido a SIGINT", Logger::get_date());
        exit_status = 6;
    } else if (signum == SIGUSR1) {
        Logger::log(prettyName(), Logger::DEBUG, "Finalizando debido a inundacion", Logger::get_date());
        exit_status = 7;
    } else {
        Logger::log(prettyName(), Logger::ERROR, "Recibi senial distinta a SIGINT y SIGUSR1", Logger::get_date());
    }
    finalize();
    exit(exit_status);
}

void MatchProcess::finalize() {
    Logger::log(prettyName(), Logger::INFO, "Finalizado", Logger::get_date());
}

// Simular el partido y dar a un equipo como ganador
void MatchProcess::run_match() {
    srand(static_cast<unsigned int>(time(0) + getpid())); // getpid evita que dos partidos que arrancan al mismo tiempo tengan el mismo resultado
    int play_time = (rand() % MAX_MATCH_DURATION) + 1;
    // Simulo que el partido dura un tiempo aleatorio entre 1 y MAX_MATCH_DURATION segundos
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
