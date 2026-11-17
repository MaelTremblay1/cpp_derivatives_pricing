#pragma once
#include <random>

// Singleton : un seul std::mt19937 pour tout le programme
class MT{
    private:
    std::mt19937 _generator;
    std::uniform_real_distribution<double> _uniform;
    std::normal_distribution<double> _normal;

    MT();
    MT(const MT&) = delete;
    MT& operator=(const MT&) = delete;
    static MT& getInstance();

    public:
    static double rand_unif();
    static double rand_norm();
};
