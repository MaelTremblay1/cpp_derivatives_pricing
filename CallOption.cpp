#include "CallOption.h"

CallOption::CallOption(double expiry, double strike): EuropeanVanillaOption(expiry, strike){}

double CallOption::payoff(double z) const{
    double K = getStrike();
    if (z >= K){
        return z - K;
    }
    else{
        return 0.0;
    }
}

optionType CallOption::GetOptionType() const{
    return optionType::call;
}
