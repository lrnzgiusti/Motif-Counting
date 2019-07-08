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
#include <csignal>
#include <sys/types.h>
#include <unistd.h>

#include "Occurrence.cpp"
using namespace std::chrono;

Estimator::Estimator(){}

Estimator::~Estimator(){}

Estimator::Estimator(Graph g){
    graph_to_sample = g;
}



Graphlet Estimator::pick_the_first(std::map<int, std::set<int>> &G, int source, int k){
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

/* Print hout a graph with a set instead of unordered set, it's only a debug function */
void tmp_out(std::unordered_map<int, std::set<int>> fast_G ){
    std::unordered_map<int, std::set<int>>::iterator it;
    for(it = fast_G.begin(); it != fast_G.end(); ++it){
        std::cout << "[" << it->first << "] -> ";
        
        std::set<int>::iterator set_iter;
        for (set_iter = it->second.begin(); set_iter != it->second.end(); ++set_iter) {
            std::cout << *set_iter << "<->";
        }
        std::cout << "NULL\n";
    }
}

volatile sig_atomic_t stop; //handle the signal

void handler(int signum){stop = 1;} //stop the chain if ctrl+c is pressed

void Estimator::sampler_test(std::map<int, std::set<int>> &G, int start, int k, int max_iter, int lock){
    //std::map<int, std::set<int>> fast_G = graph_converter(G); //fast graph (using trees instead of hashset)
    unsigned int mix_time = 0;
    Graphlet gk = Estimator().pick_the_first(G, start, k); //first graphlet i pick from G, the variable is used to point to the current graphlet
    Graphlet uk; //Graphlet I add to the final result
    
    std::set<int>::iterator it; //iterator to get the adding node
    std::unordered_map<int, std::set<int>> L; //temportary distribution
    std::map<int, std::set<int>> Lprime; //verteces whih i pick the new node randomly
    std::set<int> keys; //nodes in the graphlets
    
    std::random_device rd; //random distro stuff
    std::mt19937 gen(rd()); //random distro stuff

    std::vector<double> p; //probability to select a vertex u
    std::vector<int> q; //the corresponding vector of vertex picket with a distro specified in p;
    std::discrete_distribution<> d; //distrbution of the nodes in which i pick the including one.
    
    int excl = 0;
    int incl = 0;
    float deno = 0; //(sum L(v) for v in g)
    std::unordered_map<std::string, float> motif_distro; //result

    Occurrence *o;
    OccurrenceCanonicizer *oc;
    signal(SIGINT, handler);
    float acc = 0; //useful for statistics
    std::cout << "Starting algorithm\n";
    do{
        
        auto start = high_resolution_clock::now();
       
        //get the nodes of gk, useful for removing one of them in the next section.
        for(const std::pair<int, std::unordered_set<int>> &kv : gk) keys.insert(kv.first);
        
        //for-each vertex in the graphlet O(k)
        for(const std::pair<int, std::unordered_set<int>> &u : gk){
            
            uk = gk;
            if(uk.exclude_vertex(G, u.first)) //if gk - vk.first is not connected i avoid useless stuff to do
            {
                //L(u)= \cup{N(v) for v in g-u}
                for(const std::pair<int, std::unordered_set<int>> &v : uk){
                        std::set_union(L[u.first].begin(), L[u.first].end(),
                                       G[v.first].begin(), G[v.first].end(),
                                       std::inserter(L[u.first],
                                                     L[u.first].end()));
                    
                }
                
                //I remove the verteces that already belongs to the motif
                for(const int &key : keys){
                    L[u.first].erase(key);
                }
                deno += L[u.first].size();
            }//end if(uk.exclude_vertex(fast_G, vk.first))
        }//end for(const std::pair<int, std::unordered_set<int>> &vk : gk)
        
        
        for(const std::pair<int, std::unordered_set<int>> &u : gk){
            p.push_back(L[u.first].size()/deno);
            q.push_back(u.first);
        }
        
        d = *(new std::discrete_distribution<>(p.begin(), p.end()));
        excl = q[d(gen)];
        
        it = L[excl].begin(); //pick an iterator to the set
        std::advance(it,rand()%L[excl].size()); //pick a random neighbor
        incl = *it; //set it to the new graphlet
        gk.exclude_include_vertex(G, excl, incl); //excl == incl; perché?
        mix_time++;
        
        //update the distribution every lock steps
        if(mix_time % lock == 0){
            o= new Occurrence(gk.get_size(), &gk);
            oc = (new OccurrenceCanonicizer(gk.get_size()));
            oc->canonicize(o);
            motif_distro[o->text_footprint()] += 1.0/deno;
        }
        //clear all the temporary variables
        keys.clear();
        L.clear();
        p.clear();
        q.clear();
        deno = 0;
        
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end-start;
        
        acc += diff.count();

    }while((mix_time < max_iter) and (!stop));
    normalize_distribution(motif_distro);
    
    std::ofstream of;
    std::stringstream ss;
    ss <<std::this_thread::get_id();
    of.open(ss.str()+".txt");
    if(!of) std::cerr << "File di output non aperto correttamente\n";
    of << "Starting node: " << start << "\n";
    of << "Number of nodes in a graphlet: " << k << "\n";
    of << "Steps done: " << max_iter << "\n";
    of << "Steps before sampling: " << lock << "\n";
    of  << "Average jump time: " << acc/mix_time << "\n";
    of << "Statistics: \n\n";
    for (auto kv : motif_distro){
        of << kv.first << "\t" << kv.second << "\n";
    }
    of.close();
    return ;
}
