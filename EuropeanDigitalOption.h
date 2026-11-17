#pragma once
#include "Option.h"
#include "EuropeanVanillaOption.h"  // pour l'enum optionType

class EuropeanDigitalOption: public Option{
    private:
    double _strike;

    protected:
    double getStrike() const;

    public:
    EuropeanDigitalOption(double expiry, double strike);
    virtual optionType GetOptionType() const = 0;

    friend class BlackScholesPricer;
};
