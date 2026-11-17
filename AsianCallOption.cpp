#include "AsianCallOption.h"
#include <stdexcept>

AsianCallOption::AsianCallOption(std::vector<double> timeSteps, double strike): AsianOption(timeSteps), _strike(strike){
    if (strike < 0){
        throw std::invalid_argument("strike should be nonnegative");
    }
}

double AsianCallOption::payoff(double z) const{
    if (z >= _strike){
        return z - _strike;
    }
    else{
        return 0.0;
    }
}
