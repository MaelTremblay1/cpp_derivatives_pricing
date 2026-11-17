#include "BlackScholesMCPricer.h"
#include "AsianOption.h"
#include "MT.h"
#include <cmath>
#include <stdexcept>

BlackScholesMCPricer::BlackScholesMCPricer(Option* option, double initial_price, double interest_rate, double volatility)
    : _option(option), _initial_price(initial_price), _interest_rate(interest_rate), _volatility(volatility),
      _nbPaths(0), _estimate(0.0), _sumSquares(0.0){}

int BlackScholesMCPricer::getNbPaths() const{
    return _nbPaths;
}

void BlackScholesMCPricer::generate(int nb_paths){
    if (nb_paths < 0){
        throw std::invalid_argument("the number of paths should be nonnegative");
    }
    if (nb_paths == 0){
        return;
    }

    // dates d'observation : (t1, ..., tm) pour une asiatique, (T) sinon
    std::vector<double> timeSteps;
    if (_option->isAsianOption()){
        timeSteps = dynamic_cast<AsianOption*>(_option)->getTimeSteps();
    }
    else{
        timeSteps.push_back(_option->getExpiry());
    }

    double r = _interest_rate;
    double sigma = _volatility;
    double discount = std::exp(-r * _option->getExpiry());

    // une seule trajectoire en mémoire à la fois, réutilisée (rien n'est stocké dans l'objet)
    std::vector<double> path(timeSteps.size());
    double sum = 0.0;
    for (int p = 0; p < nb_paths; p++){
        double S = _initial_price;
        double previous = 0.0;
        for (size_t k = 0; k < timeSteps.size(); k++){
            double dt = timeSteps[k] - previous;
            S *= std::exp((r - sigma * sigma / 2) * dt + sigma * std::sqrt(dt) * MT::rand_norm());
            path[k] = S;
            previous = timeSteps[k];
        }
        double discountedPayoff = discount * _option->payoffPath(path);
        sum += discountedPayoff;
        _sumSquares += discountedPayoff * discountedPayoff;
    }

    // mise à jour de la moyenne : nouvelle = (ancienne * n + somme du lot) / (n + nb_paths)
    _estimate = (_estimate * _nbPaths + sum) / (_nbPaths + nb_paths);
    _nbPaths += nb_paths;
}

double BlackScholesMCPricer::operator()() const{
    if (_nbPaths == 0){
        throw std::logic_error("no path has been generated yet: call generate() first");
    }
    return _estimate;
}

std::vector<double> BlackScholesMCPricer::confidenceInterval() const{
    if (_nbPaths < 2){
        throw std::logic_error("at least two paths are needed to compute a confidence interval");
    }
    double n = _nbPaths;
    double variance = (_sumSquares / n - _estimate * _estimate) * n / (n - 1);  // variance empirique non biaisée
    double halfWidth = 1.96 * std::sqrt(variance / n);
    return {_estimate - halfWidth, _estimate + halfWidth};
}
