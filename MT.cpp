#include "MT.h"

MT::MT(): _generator(std::random_device{}()), _uniform(0.0, 1.0), _normal(0.0, 1.0){}

MT& MT::getInstance(){
    static MT instance;  // créée une seule fois, au premier appel
    return instance;
}

double MT::rand_unif(){
    MT& mt = getInstance();
    return mt._uniform(mt._generator);
}

double MT::rand_norm(){
    MT& mt = getInstance();
    return mt._normal(mt._generator);
}
