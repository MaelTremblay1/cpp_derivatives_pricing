#include "AsianPutOption.h"
#include <stdexcept>

AsianPutOption::AsianPutOption(std::vector<double> timeSteps, double strike): AsianOption(timeSteps), _strike(strike){
    if (strike < 0){
        throw std::invalid_argument("strike should be nonnegative");
    }
}

double AsianPutOption::payoff(double z) const{
    if (_strike >= z){
        return _strike - z;
    }
    else{
        return 0.0;
    }
}
