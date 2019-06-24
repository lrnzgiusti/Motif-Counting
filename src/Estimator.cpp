//
//  Graplets_sampler.cpp
//  dummy
//
//  Created by Lorenzo Giusti on 08/04/2019.
//  Copyright © 2019 Lorenzo Giusti. All rights reserved.
//


#include <chrono>


#include "Estimator.hpp"
#include "Utility.hpp"

using namespace std::chrono;

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


std::unordered_map<int, float> Estimator::read_distro(std::string filename){
    std::ifstream fptr;
    std::unordered_map<int, float> distro;
    fptr.open(filename);
    
    if(!fptr){
        std::cerr << "Unable to open distro file!\tTry Again!\n";
        return distro;
    }
    std::hash<Graphlet> hasher;
    std::vector<std::string> tokens;
    std::string tok1;
    std::string row;
    float tok2;
    while(std::getline(fptr, row)){
        boost::split(tokens, row, [](char c){return c == ',';});
        tok1 = tokens[0];
        tok2 = std::stof(tokens[1]);
        distro[hasher(Graphlet(tokens[0]))] = tok2;
        tokens.clear();
    }
    fptr.close();
    return distro;
}


bool exist_edge(std::unordered_map<Graphlet, std::unordered_set<Graphlet>> Gk, Graphlet u, Graphlet v){
    return ((Gk[u].find(v) != Gk[u].end()) and (Gk[v].find(u) != Gk[v].end()));
}

//This function is used for estimate the graph of graphlets distribution
std::unordered_map<Graphlet, float> Estimator::sampler(Graph G, int start, int k){
    //std::cout << "spia: sampling started\n";
    std::unordered_map<Graphlet, std::unordered_set<Graphlet>> Gk; //the final Graph of graphlets
    std::unordered_map<Graphlet, float> distro_t; //the current distribution
    std::unordered_map<Graphlet, float> distro_tprec; //the distribution at t-1 for make the comparisons
    unsigned int mix_time = 1;
    float epsilon = 0.005; //precision to declare convergence
    Graphlet gk = Estimator().pick_the_first(G, start, k); //first graphlet i pick from G, the variable is used to point to the current graphlet
    Graphlet uk; //Graphlet I add to the final result
    distro_t[gk] = 1; //init of the distribution
    std::cout << "starting sampling\n";
    do{
        distro_tprec = distro_t; //alignment
        for(std::pair<int, std::unordered_set<int>> vk : gk){ //for-each vertex in the graphlet
            for(std::pair<int, std::unordered_set<int>> wk : gk){ //for-each vertex in the graphlet without the previous
                
                if(vk.first != wk.first){ //this implies that in this inner iteration you exclude vk
                    for(int nk : G[wk.first]){ //for-each neighbor of wk in the original graph
                        // (vk.first != nk) means that i don't insert the vertex i'm excluding
                        // (gk.get_repr().find(nk) == gk.end()) means that i don't insert a vertex already in the graphlet
                        if((vk.first != nk) and (gk.exist_vertex(nk) == false)){
                            
                            uk = gk;
                            
                            uk.exclude_include_vertex(G, vk.first, nk); //molto lento
                            
                            auto start = high_resolution_clock::now();
                            //durata di questo check è incrementale con il numero di iterazioni, perch?
                            if (!(exist_edge(Gk, uk, gk)) and uk.isConnected()) { //cercare di velocizzare
                                
                                
                                auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start);
                                std::cout << "Time taken by function: "<< duration.count() << " ms\n";
                                Gk[uk].insert(gk);
                                Gk[gk].insert(uk);
                                
                            }
                            /*
                             */
                        }
                        
                    }
                }
            }
        }
        
        if (Gk[gk].size() > 0 )
            gk = *(std::next(Gk[gk].begin(), rand()%Gk[gk].size()));
        distro_t[gk] += 1;
        distro_tprec[gk] += 0;
        mix_time++;
        std::cout << mix_time << " mix\n";
    }while(mix_time < 100);//(l1_diff(distro_t, distro_tprec, mix_time) >= epsilon);
    normalize_distribution(distro_t, mix_time);
    return distro_t;
}

//this function estimate the mixing time
std::unordered_map<Graphlet, float> Estimator::random_walk(Graph G, int start, int k){
    std::unordered_map<Graphlet, std::unordered_set<Graphlet>> Gk; //the final Graph of graphlets
    std::unordered_map<int, float> distro_t; //the current distribution
    std::unordered_map<Graphlet, float> distro_t_graphlets; //the current distribution
    std::unordered_map<int, float> distro_real = read_distro("/Users/ince/Desktop/bressan_data/distro_3.dat");
    int t = 1; //current time
    std::hash<Graphlet> hasher;
    float epsilon = 0.055; //precision to declare convergence
    Graphlet gk = Estimator().pick_the_first(G, start, k); //first graphlet i pick from G, the variable is used to point to the current graphlet
    Graphlet uk; //Graphlet I add to the final result
    distro_t[hasher(gk)] = 1; //init of the distribution
    distro_t_graphlets[gk] = 1;
    do{
        t++;
        //distro_tprec = distro_t; //alignment
        for(std::pair<int, std::unordered_set<int>> vk : gk){ //for-each vertex in the graphlet
            for(std::pair<int, std::unordered_set<int>> wk : gk){ //for-each vertex in the graphlet without the previous
                if(vk.first != wk.first){ //this implies that in this inner iteration you exclude vk
                    for(int nk : G[wk.first]){ //for-each neighbor of wk in the original graph
                        // (vk.first != nk) means that i don't insert the vertex i'm excluding
                        // (gk.get_repr().find(nk) == gk.end()) means that i don't insert a vertex already in the graphlet
                        
                        if((vk.first != nk) and (gk.get_repr().find(nk) == gk.end())){
                            uk = gk;
                            uk.exclude_include_vertex(G, vk.first, nk);
                            if (uk.isConnected()) {
                                Gk[uk].insert(gk);
                                Gk[gk].insert(uk);
                            }
                        }
                    }
                }
            }
        }
        if (Gk[gk].size() > 0 )
            gk = *(std::next(Gk[gk].begin(), rand()%Gk[gk].size()));
        
        distro_t[hasher(gk)] += 1;
        distro_t_graphlets[gk] += 1;
    }while(l1_disto_diff(distro_t, distro_real, t) > epsilon);
    //maybe insert the L1 distance between the two distributions, some eustistics
    normalize_distribution(distro_t, t);
    normalize_distribution(distro_t_graphlets, t);
    std::cout << "Mixing time: " << t << "\n";
    return distro_t_graphlets;
}
