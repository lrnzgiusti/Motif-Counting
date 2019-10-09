//
//  Graplets_sampler.hpp
//  dummy
//
//  Created by Lorenzo Giusti on 08/04/2019.
//  Copyright © 2019 Lorenzo Giusti. All rights reserved.
//

#ifndef Estimator_hpp
#define Estimator_hpp

#include "Graphlet.cpp"
class Estimator{
public:
    Estimator();
    
    ~Estimator();
    Graphlet pick_the_first(Graph &, int, int); // this will be called at the start of the random walk so it's private. Public function only for the tests.
    
    void sampler(Graph &G, int start, int k, int , int);
    void sampler_weighted(Graph &G, int start, int k, int max_iter, int lock);

    
private:
    int k;
    
    
};

#endif /* Graplets_sampler_hpp */
