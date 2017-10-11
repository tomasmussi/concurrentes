#include "ResultsReporter.h"
#include "../ipc/SignalHandler.h"
#include "../utils/Logger.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>

ResultsReporter::ResultsReporter(const std::string& fifo_read):
    _fifo_read(fifo_read), _countdown(RESULTS_TABLE_FREQUENCY){
}

ResultsReporter::~ResultsReporter() {
}

int ResultsReporter::do_work() {
    Match match;
    _fifo_read.leer(static_cast<void*>(&match), sizeof(Match));
    // Esto es porque cuando recibe el SIGINT, lee un partido invalido
    if (match.valid()) {
        _countdown--;
        Logger::log(prettyName(), Logger::INFO, match.to_string(), Logger::get_date());
        updateTableWithMatch(match);
        if (_countdown == 0){
            //Mostrar Tabla de Posiciones
            showPointsTable();
            _countdown = RESULTS_TABLE_FREQUENCY;
        }
    }
    return 0;
}

void ResultsReporter::initialize() {
    Logger::log(prettyName(), Logger::DEBUG, "Inicializando", Logger::get_date());
    _fifo_read.abrir();
    Logger::log(prettyName(), Logger::DEBUG, "Fifo de lectura de resultados de ResultsReporter abierto", Logger::get_date());
    Logger::log(prettyName(), Logger::INFO, "Inicializado", Logger::get_date());
}

void ResultsReporter::finalize() {
    Logger::log(prettyName(), Logger::DEBUG, "Finalizando", Logger::get_date());
    // Muestro tabla de resultados al recibir SIGINT
    showPointsTable();
    showChampion();
    _fifo_read.cerrar();
    Logger::log(prettyName(), Logger::DEBUG, "Fifo cerrado", Logger::get_date());
    Logger::log(prettyName(), Logger::INFO, "Finalizado", Logger::get_date());
}

void ResultsReporter::updateTableWithMatch(Match match){
    Team team1 = match.team1();
    Team team2 = match.team2();
    int pointsTeam1 = pointsFromScore(match.scoreTeam1(), match.scoreTeam2());
    int pointsTeam2 = pointsFromScore(match.scoreTeam2(), match.scoreTeam1());
    updateTableWithTeam(team1, pointsTeam1);
    updateTableWithTeam(team2, pointsTeam2);
}

void ResultsReporter::updateTableWithTeam(Team team, int points){
    updateTableWithPerson(team.get_person1(), points);
    updateTableWithPerson(team.get_person2(), points);
}

void ResultsReporter::updateTableWithPerson(Person p, int points){
    std::string id = p.id();
    //Es el primer partido que jugo
    if ( _pointsTable.count(id) == 0){
        player_t p;
        p.points = points;
        p.matchesPlayed = 1;
        _pointsTable[id] = p;
    }
    else{
        _pointsTable[id].points += points;
        _pointsTable[id].matchesPlayed += 1;
    }
}

/**
 * Recordar:
 * 3-0 y 3-1 son 3 puntos ganadores y 0 puntos perdedores
 * 3-2 son 2 puntos ganadores y 1 punto perdedores
 * */
int ResultsReporter::pointsFromScore(int score, int opponent_score) const {
    if (score == 0 || score == 1){
        return 0;
    } else if (score == 2){
        return 1;
    }
    // El score es igual a 3
    if (opponent_score == 2) {
        return 2;
    } else {
        return 3;
    }
}

void ResultsReporter::showPointsTable() {
    std::vector<std::pair<std::string, player_t> > elems(_pointsTable.begin(), _pointsTable.end());
    std::sort(elems.begin(), elems.end(), comp);
    std::stringstream ss;
    ss << "Tabla de Posiciones" <<std::endl;
    for (std::vector<std::pair<std::string, player_t> >::iterator it = elems.begin(); it != elems.end(); ++it){
        ss <<"Participante: " << it->first << " | Puntos: " << it->second.points << " | Partidos: " << it->second.matchesPlayed << std::endl;
    }
    Logger::log(prettyName(), Logger::INFO, ss.str(), Logger::get_date());
}

void ResultsReporter::showChampion() {
    std::vector<std::pair<std::string, player_t> > elems(_pointsTable.begin(), _pointsTable.end());
    std::sort(elems.begin(), elems.end(), comp);
    std::stringstream ss;
    int maxPoints;
    std::vector<std::pair<std::string, player_t> >::iterator it = elems.begin();
    if (it != elems.end()) {
        maxPoints = it->second.points;
    }
    ss << "Campeón(es) con "<< maxPoints << " puntos: " << std::endl;
    for (; it != elems.end(); ++it){
        if (it->second.points == maxPoints) {
            ss << "\tParticipante: " << it->first << std::endl;
        } else {
            break;
        }
    }
    Logger::log(prettyName(), Logger::INFO, ss.str(), Logger::get_date());
}

std::string ResultsReporter::prettyName() {
    return "Results Reporter";
}
