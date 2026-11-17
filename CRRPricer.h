#pragma once
#include "Option.h"
#include "BinaryTree.h"

class CRRPricer{
    private:
    Option* _option;
    int _depth;
    double _asset_price;
    double _up;
    double _down;
    double _interest_rate;
    BinaryTree<double> _prices;    // H(n, i)
    BinaryTree<bool> _exercise;    // politique d'exercice (options américaines)
    bool _computed;

    double stockPrice(int n, int i) const;  // S(n, i)
    double riskNeutralProbability() const;  // q

    public:
    CRRPricer(Option* option, int depth, double asset_price, double up, double down, double interest_rate);
    CRRPricer(Option* option, int depth, double asset_price, double r, double volatility);

    void compute();
    double get(int n, int i);
    bool getExercise(int n, int i);
    double operator()(bool closed_form = false);
};
