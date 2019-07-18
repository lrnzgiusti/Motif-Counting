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
    Graphlet pick_the_first(std::map<int, std::set<int>> &, int, int); // this will be called at the start of the random walk so it's private. Public function only for the tests.
    
    void sampler(std::map<int, std::set<int>> &G, int start, int k, int , int);
    
    
private:
    int k;
    
    
};

#endif /* Graplets_sampler_hpp */
