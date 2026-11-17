#pragma once
#include <vector>

class Option{
    private:
    double _expiry;

    public:
    Option(double expiry);
    virtual ~Option() {}

    double getExpiry() const;
    virtual double payoff(double z) const = 0;

    // h(S_t1, ..., S_tm) ; par défaut h(S_tm) pour une option européenne
    virtual double payoffPath(const std::vector<double>& path) const;

    virtual bool isAsianOption() const;
    virtual bool isAmericanOption() const;
};
