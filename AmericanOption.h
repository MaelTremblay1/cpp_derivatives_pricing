#pragma once
#include "Option.h"

class AmericanOption: public Option{
    private:
    double _strike;

    protected:
    double getStrike() const;

    public:
    AmericanOption(double expiry, double strike);
    bool isAmericanOption() const override;
};
