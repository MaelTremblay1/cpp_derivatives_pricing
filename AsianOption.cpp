#include "AsianOption.h"
#include <stdexcept>

namespace{
    // vérifie (t1, ..., tm) avant d'appeler le constructeur d'Option, qui a besoin de tm
    double lastTimeStep(const std::vector<double>& timeSteps){
        if (timeSteps.empty()){
            throw std::invalid_argument("an Asian option needs at least one time step");
        }
        double previous = 0.0;
        for (double t : timeSteps){
            if (t <= previous){
                throw std::invalid_argument("time steps should be positive and increasing");
            }
            previous = t;
        }
        return timeSteps.back();
    }
}

AsianOption::AsianOption(std::vector<double> timeSteps): Option(lastTimeStep(timeSteps)), _timeSteps(timeSteps){}

std::vector<double> AsianOption::getTimeSteps() const{
    return _timeSteps;
}

double AsianOption::payoffPath(const std::vector<double>& path) const{
    if (path.empty()){
        throw std::invalid_argument("the path should not be empty");
    }
    double sum = 0.0;
    for (double S : path){
        sum += S;
    }
    return payoff(sum / path.size());
}

bool AsianOption::isAsianOption() const{
    return true;
}
