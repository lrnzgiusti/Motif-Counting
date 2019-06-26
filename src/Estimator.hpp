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
    Estimator(Graph); //deprecated
    ~Estimator();
    Graphlet pick_the_first(Graph &, int, int); // this will be called at the start of the random walk so it's private. Public function only for the tests.
    std::unordered_map<Graphlet, float> random_walk(Graph G, int start, int k);
    
    std::unordered_map<int, float> read_distro(std::string);
    std::unordered_map<Graphlet, float>  sampler(Graph &G, int start, int k);
    
    
private:
    int k;
    Graph graph_to_sample;
    std::unordered_map<Graphlet, std::unordered_set<Graphlet>> repr;
};

#endif /* Graplets_sampler_hpp */
