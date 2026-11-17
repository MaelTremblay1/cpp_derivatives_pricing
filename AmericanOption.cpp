#include "AmericanOption.h"
#include <stdexcept>

AmericanOption::AmericanOption(double expiry, double strike): Option(expiry), _strike(strike){
    if (expiry < 0 || strike < 0){
        throw std::invalid_argument("expiry and strike should be nonnegative");
    }
}

double AmericanOption::getStrike() const{
    return _strike;
}

bool AmericanOption::isAmericanOption() const{
    return true;
}
