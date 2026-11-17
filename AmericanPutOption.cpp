#include "AmericanPutOption.h"

AmericanPutOption::AmericanPutOption(double expiry, double strike): AmericanOption(expiry, strike){}

double AmericanPutOption::payoff(double z) const{
    double K = getStrike();
    if (K >= z){
        return K - z;
    }
    else{
        return 0.0;
    }
}
