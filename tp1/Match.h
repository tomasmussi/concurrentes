//
// Created by tomas on 11/09/17.
//

#ifndef TP1_MATCH_H
#define TP1_MATCH_H


#include <list>
#include <map>

#define MATCH_PROBABILITY 50

/**
 * Proceso encargado de recibir dos parejas de equipos
 * y realizar la simulacion del partido y devolver un resultado
 * con los puntos que gano cada uno de los integrantes y el partido como salio
 * */
class Match {
private:
    std::list<int> _team1;
    std::list<int> _team2;
    int _row;
    int _column;
    float _probability;
    int _score_team1;
    int _score_team2;
    void run_match();
    void set_scores(int& score_winner, int& score_loser);
public:
    Match(const std::list<int>& team1, const std::list<int>& team2, int row, int column);
    Match(const std::list<int>& team1, const std::list<int>& team2, int row, int column, float probability);
    std::map<int, int> get_individual_scores();
    int get_team1_score() const;
    int get_team2_score() const;
    std::list<int> get_team1() const;
    std::list<int> get_team2() const;

};


#endif //TP1_MATCH_H
