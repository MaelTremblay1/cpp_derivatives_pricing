#pragma once
#include "Option.h"
#include <vector>

class BlackScholesMCPricer{
    private:
    Option* _option;
    double _initial_price;
    double _interest_rate;
    double _volatility;
    int _nbPaths;            // nombre de trajectoires déjà simulées
    double _estimate;        // moyenne courante des payoffs actualisés
    double _sumSquares;      // somme des carrés des payoffs actualisés (pour l'IC)

    public:
    BlackScholesMCPricer(Option* option, double initial_price, double interest_rate, double volatility);
    int getNbPaths() const;
    void generate(int nb_paths);
    double operator()() const;
    std::vector<double> confidenceInterval() const;
};
