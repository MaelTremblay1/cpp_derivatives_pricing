#include "AmericanCallOption.h"

AmericanCallOption::AmericanCallOption(double expiry, double strike): AmericanOption(expiry, strike){}

double AmericanCallOption::payoff(double z) const{
    double K = getStrike();
    if (z >= K){
        return z - K;
    }
    else{
        return 0.0;
    }
}
