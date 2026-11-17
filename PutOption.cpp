#include "PutOption.h"

PutOption::PutOption(double expiry, double strike): EuropeanVanillaOption(expiry, strike){}

double PutOption::payoff(double z) const{
    double K = getStrike();
    if (K >= z){
        return K - z;
    }
    else{
        return 0.0;
    }
}

optionType PutOption::GetOptionType() const{
    return optionType::put;
}
