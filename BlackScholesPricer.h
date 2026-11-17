#pragma once
#include "EuropeanVanillaOption.h"
#include "EuropeanDigitalOption.h"

class BlackScholesPricer{
    private:
    Option* _option;
    double _strike;
    optionType _type;
    bool _isDigital;
    double _asset_price;
    double _interest_rate;
    double _volatility;

    double d1() const;
    double d2() const;
    static double normalCDF(double x);
    static double normalPDF(double x);

    public:
    BlackScholesPricer(EuropeanVanillaOption* option, double asset_price, double interest_rate, double volatility);
    BlackScholesPricer(EuropeanDigitalOption* option, double asset_price, double interest_rate, double volatility);
    double operator()() const;
    double delta() const;
};
