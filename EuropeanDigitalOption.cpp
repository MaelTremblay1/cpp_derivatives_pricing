#include "EuropeanDigitalOption.h"
#include <stdexcept>

EuropeanDigitalOption::EuropeanDigitalOption(double expiry, double strike): Option(expiry), _strike(strike){
    if (expiry < 0 || strike < 0){
        throw std::invalid_argument("expiry and strike should be nonnegative");
    }
}

double EuropeanDigitalOption::getStrike() const{
    return _strike;
}
