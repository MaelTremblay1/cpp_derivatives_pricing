#include "BlackScholesPricer.h"
#include <cmath>

BlackScholesPricer::BlackScholesPricer(EuropeanVanillaOption* option, double asset_price, double interest_rate, double volatility)
    : _option(option), _strike(option->_strike), _type(option->GetOptionType()), _isDigital(false),
      _asset_price(asset_price), _interest_rate(interest_rate), _volatility(volatility){}

BlackScholesPricer::BlackScholesPricer(EuropeanDigitalOption* option, double asset_price, double interest_rate, double volatility)
    : _option(option), _strike(option->_strike), _type(option->GetOptionType()), _isDigital(true),
      _asset_price(asset_price), _interest_rate(interest_rate), _volatility(volatility){}

double BlackScholesPricer::d1() const{
    double T = _option->getExpiry();
    double sigma = _volatility;
    return (std::log(_asset_price / _strike) + (_interest_rate + sigma * sigma / 2) * T) / (sigma * std::sqrt(T));
}

double BlackScholesPricer::d2() const{
    return d1() - _volatility * std::sqrt(_option->getExpiry());
}

double BlackScholesPricer::normalCDF(double x){
    return 0.5 * std::erfc(-x / std::sqrt(2.0));
}

double BlackScholesPricer::normalPDF(double x){
    const double pi = 3.14159265358979323846;
    return std::exp(-x * x / 2) / std::sqrt(2 * pi);
}

double BlackScholesPricer::operator()() const{
    double S = _asset_price;
    double K = _strike;
    double discount = std::exp(-_interest_rate * _option->getExpiry());

    if (_isDigital){
        // digitale : paie 1 si l'option finit dans la monnaie
        if (_type == optionType::call){
            return discount * normalCDF(d2());
        }
        else{
            return discount * normalCDF(-d2());
        }
    }

    if (_type == optionType::call){
        return S * normalCDF(d1()) - K * discount * normalCDF(d2());
    }
    else{
        return K * discount * normalCDF(-d2()) - S * normalCDF(-d1());
    }
}

double BlackScholesPricer::delta() const{
    if (_isDigital){
        double T = _option->getExpiry();
        double discount = std::exp(-_interest_rate * T);
        double deltaCall = discount * normalPDF(d2()) / (_volatility * _asset_price * std::sqrt(T));
        if (_type == optionType::call){
            return deltaCall;
        }
        else{
            return -deltaCall;
        }
    }

    if (_type == optionType::call){
        return normalCDF(d1());
    }
    else{
        return normalCDF(d1()) - 1.0;
    }
}
