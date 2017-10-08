#include "ResultsReporter.h"
#include "../ipc/SignalHandler.h"
#include "../utils/Logger.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>


ResultsReporter::ResultsReporter(const std::string& fifo_read):
    _fifo_read(fifo_read), _countdown(FREQUENCY){
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
            _countdown = FREQUENCY;
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
    _fifo_read.cerrar();
    Logger::log(prettyName(), Logger::DEBUG, "Fifo cerrado", Logger::get_date());
    Logger::log(prettyName(), Logger::INFO, "Finalizado", Logger::get_date());
}

void ResultsReporter::updateTableWithMatch(Match match){
    Team team1 = match.team1();
    Team team2 = match.team2();
    int pointsTeam1 = pointsFromScore(match.scoreTeam1());
    int pointsTeam2 = pointsFromScore(match.scoreTeam2());
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
        _pointsTable[id] = points;
    }
    else{
        _pointsTable[id] += points;
    }
}

int ResultsReporter::pointsFromScore(int score) const {
    if (score == 0 || score == 1){
        return 0;
    }
    else if (score == 2){
        return 1;
    }
    else{
        return 3;
    }
}

void ResultsReporter::showPointsTable() {
    std::vector<std::pair<std::string, int> > elems(_pointsTable.begin(), _pointsTable.end());
    std::sort(elems.begin(), elems.end(), comp);
    std::stringstream ss;
    ss << "Tabla de Posiciones" <<std::endl;
    for (std::vector<std::pair<std::string, int> >::const_iterator it = elems.begin(); it != elems.end(); ++it){
        ss <<"Participante: " << it->first << " | Puntos: " << it->second << std::endl;
    }
    Logger::log(prettyName(), Logger::INFO, ss.str(), Logger::get_date());
}

std::string ResultsReporter::prettyName() {
    return "Results Reporter";
}
