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
    std::set<int> Vk = {source}; //vertex in the first graphlet
    std::set<std::pair<int, int>> Ek; //edges in the first graphlet
    std::set<int> C;
    std::vector<int> Vk_diff_C(1); // (V \ C) verteces for which i have not seen their neighbours
    
    std::set<int> neighbours_v; //neighbours of the current vetex --probably useless
    std::set<int> Vk_intersect_neighbours; // (Vk n Neighbours(v)) this implies that i check the neighbours of v if and only if all of them are not in the resulting graphlet
    std::vector<int> neighbours_diff_V(1); //I take a neghbour of v if is not inside Vk
    
    int rng; //the random index picked
    int v, v_prime; //the vertex on which you start the traversal
    
    while (Vk.size() < k) {
        
        std::set_difference(Vk.begin(), Vk.end(),
                            C.begin(), C.end(), Vk_diff_C.begin()); //compute Vk \ C
        rng = rand()%Vk_diff_C.size(); //pick an index
        v = Vk_diff_C[rng]; //pick the associated value
        C.insert(v); //insert the value into checked values
        neighbours_v.insert(G[v].begin(), G[v].end() );
        
        std::set_intersection(neighbours_v.begin(), neighbours_v.end(),
                              Vk.begin(), Vk.end(),
                              std::inserter(Vk_intersect_neighbours,
                                            Vk_intersect_neighbours.end())); //compute ( (Vk n Neighbours(v)) )
        
        while ((Vk.size() < k) and (Vk_intersect_neighbours != neighbours_v)) {
            
            neighbours_diff_V.clear();
            //compte Neighbours(v) \ Vk
            std::set_difference(neighbours_v.begin(), neighbours_v.end(),
                                Vk.begin(), Vk.end(), std::inserter(neighbours_diff_V, neighbours_diff_V.end()));
            
            if(neighbours_diff_V.size() == 0) //avoid math error, it says that all the neighbours are into Vk
                break;
            
            rng = rand()%neighbours_diff_V.size(); //pick an index
            v_prime = neighbours_diff_V[rng]; //pick the associated value
            
            //search the neighbours of v_prime that i've taken in my graphlet and insert eventual edges
            for (int v_second : Vk) {
                if(G[v_prime].find(v_second) != G[v_prime].end()){
                    Ek.insert(std::make_pair(v_prime, v_second));
                    Vk.insert(v_second);
                }
            }
            
            //insert only here v_prime so you don't see it when search the v_seconds
            Vk.insert(v_prime);
            
            Vk_intersect_neighbours.clear();
            std::set_intersection(G[v].begin(), G[v].end(),
                                  Vk.begin(), Vk.end(),
                                  std::inserter(Vk_intersect_neighbours,
                                                Vk_intersect_neighbours.end())); //compute ( (Vk n Neighbours(v)) )
        }
    }
    
    return *new Graphlet(Ek);
}

/**
 * This function implements the Markov Chain
 * param:
 * return:
 */
std::unordered_map<Graphlet, std::unordered_set<Graphlet>> random_walk(Graph G, int start, int k){
    std::unordered_map<Graphlet, std::unordered_set<Graphlet>> Gk;
    std::vector<float> distro_t;
    std::vector<float> distro_tprime;
    int t = 1;
    float epsilon = 1e-5;
    Graphlet gk = Estimator().pick_the_first(G, 1, 3);
    do{
        t++;
        
    }while(l1_diff(distro_t, distro_tprime, t) > epsilon);
    
    return Gk;
    
}
