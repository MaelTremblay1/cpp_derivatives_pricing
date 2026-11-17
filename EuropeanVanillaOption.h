#pragma once
#include "Option.h"

enum class optionType {call, put};

class EuropeanVanillaOption: public Option{
    private:
    double _strike;

    protected:
    double getStrike() const;

    public:
    EuropeanVanillaOption(double expiry, double strike);
    virtual optionType GetOptionType() const = 0;

    friend class BlackScholesPricer;
};
