//
//  Graplets_sampler.cpp
//  dummy
//
//  Created by Lorenzo Giusti on 08/04/2019.
//  Copyright © 2019 Lorenzo Giusti. All rights reserved.
//

#include "Estimator.hpp"
#include "Utility.hpp"

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

std::unordered_map<Graphlet, std::unordered_set<Graphlet>> Estimator::random_walk_test(Graph G, int start, int k){
    std::unordered_map<Graphlet, std::unordered_set<Graphlet>> Gk; //the final Graph of graphlets
    std::unordered_map<Graphlet, float> distro_t; //the current distribution
    std::unordered_map<Graphlet, float> distro_tprec; //the distribution at t-1 for make the comparisons
    
    int t = 1; //current time
    float epsilon = 1e-4; //precision to declare convergence
    Graphlet gk = Estimator().pick_the_first(G, 1, 3); //first graphlet i pick from G, the variable is used to point to the current graphlet
    Graphlet uk; //Graphlet I add to the final result
    distro_t[gk] = 1; //init of the distribution
    do{
        t++;
         distro_tprec = distro_t; //alignment
        for(std::pair<int, std::unordered_set<int>> vk : gk){ //for-each vertex in the graphlet
            for(std::pair<int, std::unordered_set<int>> wk : gk){ //for-each vertex in the graphlet without the previous
                if(vk.first != wk.first){ //this implies that in this inner iteration you exclude vk
                    for(int nk : G[wk.first]){ //for-each neighbor of wk in the original graph
                        // (vk.first != nk) means that i don't insert the vertex i'm excluding
                        // (gk.get_repr().find(nk) == gk.end()) means that i don't insert a vertex already in the graphlet
                        if((vk.first != nk) and (gk.get_repr().find(nk) == gk.end())){
                            uk = gk.exclude_include_vertex(G, vk.first, nk);
                            if (uk.isConnected(uk.begin()->first)) {
                                Gk[uk].insert(gk);
                                Gk[gk].insert(uk);
                            }
                        }
                    }
                }
            }
        }

        gk = *(std::next(Gk[gk].begin(), rand()%Gk[gk].size()));
        distro_t[gk] += 1;
        distro_tprec[gk] += 0;
    }while(t < 1000);//(l1_diff(distro_t, distro_tprec, t) > epsilon); //convergence condition
    
    std::cout << "FINISH, T: "<< t << "\n";
    return Gk;
    
}
