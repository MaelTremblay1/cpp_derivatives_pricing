#pragma once
#include "Option.h"
#include <vector>

class AsianOption: public Option{
    private:
    std::vector<double> _timeSteps;  // (t1, ..., tm)

    public:
    AsianOption(std::vector<double> timeSteps);
    std::vector<double> getTimeSteps() const;

    // h(S_t1, ..., S_tm) = h((1/m) * somme des S_tk), avec h = payoff(double)
    double payoffPath(const std::vector<double>& path) const override;
    bool isAsianOption() const override;
};
