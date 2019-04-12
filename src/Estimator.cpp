//
//  Graplets_sampler.cpp
//  dummy
//
//  Created by Lorenzo Giusti on 08/04/2019.
//  Copyright © 2019 Lorenzo Giusti. All rights reserved.
//

#include "Estimator.hpp"
#include <boost/container/set.hpp>

Estimator::Estimator(){}

Estimator::~Estimator(){}

Estimator::Estimator(Graph g){
    graph_to_sample = g;
}



Graphlet Estimator::pick_the_first(Graph G, int source, int k){
    Graphlet gk = *new Graphlet();
    return gk;
}
