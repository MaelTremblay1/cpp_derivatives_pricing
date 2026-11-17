#include "CRRPricer.h"
#include <cmath>
#include <stdexcept>

namespace{
    // Paramètres du modèle binomial qui approxime Black-Scholes (pas h = T / N)
    double timeStep(Option* option, int depth){
        if (depth <= 0){
            throw std::invalid_argument("the depth should be positive");
        }
        return option->getExpiry() / depth;
    }

    double bsUp(Option* option, int depth, double r, double sigma){
        double h = timeStep(option, depth);
        return std::exp((r + sigma * sigma / 2) * h + sigma * std::sqrt(h)) - 1;
    }

    double bsDown(Option* option, int depth, double r, double sigma){
        double h = timeStep(option, depth);
        return std::exp((r + sigma * sigma / 2) * h - sigma * std::sqrt(h)) - 1;
    }

    double bsRate(Option* option, int depth, double r){
        return std::exp(r * timeStep(option, depth)) - 1;
    }
}

CRRPricer::CRRPricer(Option* option, int depth, double asset_price, double up, double down, double interest_rate)
    : _option(option), _depth(depth), _asset_price(asset_price), _up(up), _down(down),
      _interest_rate(interest_rate), _computed(false)
{
    if (option->isAsianOption()){
        throw std::invalid_argument("Asian options cannot be priced with the CRR model");
    }
    if (depth < 0){
        throw std::invalid_argument("the depth should be nonnegative");
    }
    if (asset_price <= 0){
        throw std::invalid_argument("the asset price should be positive");
    }
    if (down <= -1){
        throw std::invalid_argument("down should be greater than -1");
    }
    // pas d'arbitrage si et seulement si D < R < U
    if (!(down < interest_rate && interest_rate < up)){
        throw std::invalid_argument("arbitrage opportunity: D < R < U is not satisfied");
    }

    _prices.setDepth(depth);
    _exercise.setDepth(depth);
}

CRRPricer::CRRPricer(Option* option, int depth, double asset_price, double r, double volatility)
    : CRRPricer(option, depth, asset_price,
                bsUp(option, depth, r, volatility),
                bsDown(option, depth, r, volatility),
                bsRate(option, depth, r)){}

double CRRPricer::stockPrice(int n, int i) const{
    return _asset_price * std::pow(1 + _up, i) * std::pow(1 + _down, n - i);
}

double CRRPricer::riskNeutralProbability() const{
    return (_interest_rate - _down) / (_up - _down);
}

void CRRPricer::compute(){
    double q = riskNeutralProbability();
    bool american = _option->isAmericanOption();

    // à l'échéance : H(N, i) = h(S(N, i))
    for (int i = 0; i <= _depth; i++){
        double payoff = _option->payoff(stockPrice(_depth, i));
        _prices.setNode(_depth, i, payoff);
        _exercise.setNode(_depth, i, american && payoff > 0);
    }

    // induction rétrograde
    for (int n = _depth - 1; n >= 0; n--){
        for (int i = 0; i <= n; i++){
            double continuation = (q * _prices.getNode(n + 1, i + 1) + (1 - q) * _prices.getNode(n + 1, i))
                                  / (1 + _interest_rate);
            if (american){
                double intrinsic = _option->payoff(stockPrice(n, i));
                bool exercise = intrinsic >= continuation;
                _exercise.setNode(n, i, exercise);
                _prices.setNode(n, i, exercise ? intrinsic : continuation);
            }
            else{
                _exercise.setNode(n, i, false);
                _prices.setNode(n, i, continuation);
            }
        }
    }

    _computed = true;
}

double CRRPricer::get(int n, int i){
    if (!_computed){
        compute();
    }
    return _prices.getNode(n, i);
}

bool CRRPricer::getExercise(int n, int i){
    if (!_computed){
        compute();
    }
    return _exercise.getNode(n, i);
}

double CRRPricer::operator()(bool closed_form){
    if (!closed_form){
        return get(0, 0);
    }

    if (_option->isAmericanOption()){
        throw std::logic_error("the closed-form formula only applies to European options");
    }

    // H(0,0) = 1/(1+R)^N * somme_i C(N,i) q^i (1-q)^(N-i) h(S(N,i))
    // le coefficient binomial est calculé en log pour éviter les dépassements quand N est grand
    double q = riskNeutralProbability();
    int N = _depth;
    double sum = 0.0;
    for (int i = 0; i <= N; i++){
        double logWeight = std::lgamma(N + 1.0) - std::lgamma(i + 1.0) - std::lgamma(N - i + 1.0)
                           + i * std::log(q) + (N - i) * std::log(1 - q);
        sum += std::exp(logWeight) * _option->payoff(stockPrice(N, i));
    }
    return sum / std::pow(1 + _interest_rate, N);
}
