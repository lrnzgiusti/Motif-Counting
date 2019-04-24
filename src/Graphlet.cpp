//
//  Graphlet.cpp
//  dummy
//
//  Created by Lorenzo Giusti on 09/04/2019.
//  Copyright © 2019 Lorenzo Giusti. All rights reserved.
//

#include "Graphlet.hpp"
#include <math.h>
Graphlet::Graphlet(){}

Graphlet::Graphlet(std::unordered_map<int, std::unordered_set<int>> repr) : Graph(repr){}

Graphlet::Graphlet(std::set<std::pair<int, int>> edges) : Graph(edges){}

Graphlet::~Graphlet(){}



bool Graphlet::operator ==(const Graphlet &g) const{
    std::unordered_map<int, std::unordered_set<int>> other_graph_representation = g.get_repr();
    //Better handle this, avoidable but...
    /*if (num_of_edges != g.num_of_edges) {
        std:: cout << "edges: " << num_of_edges << " : " << g.num_of_edges;
        return false;
    }
    if (num_of_nodes != g.num_of_nodes) {
        std:: cout << "nodes: " << num_of_nodes << " : " << g.num_of_nodes;
        return false;
    }*/
    for (std::pair<int, std::unordered_set<int>> p : other_graph_representation) {
        try {
            if (repr.at(p.first) != other_graph_representation.at(p.first)) return false;
        } catch (...) {
            return false;
        }
    }
    return true;
}

std::unordered_map<int, std::unordered_set<int>>::iterator Graphlet::begin(){
    return repr.begin();
}

std::unordered_map<int, std::unordered_set<int>>::iterator Graphlet::end(){
    return repr.end();
}

//TODO: improve this, we have to exclude <excl> from all the neighbors and include all possible edges of incl
//TO Make this works well, we have to create a graphlet based on the original graph
//Pass a set of vertex in the constructor and then return the associated graph
Graphlet Graphlet::exclude_include_vertex(Graph G, int excl, int incl){
    Graphlet g_prime = *new Graphlet(this->repr);
    g_prime.repr.erase(excl);
    for(std::pair<int, std::unordered_set<int>> check_edges : g_prime){
        g_prime.repr[check_edges.first].erase(excl);
    }
    
    for (int vertex : G[incl]) {
        if(g_prime.repr.find(vertex) != g_prime.repr.end()){
            g_prime.repr[incl].insert(vertex);
            g_prime.repr[vertex].insert(incl);
        }
    }
    return g_prime;
}

void Graphlet::insert_edge(std::pair<int, int> incl){
    repr[incl.first].insert(incl.second);
    repr[incl.second].insert(incl.first);
}




namespace std {
    // we'll use this for make sets and maps of graphlets, needed for GRASP patterns
    
    template<>
    struct hash<Graphlet>
    {
        std::size_t operator()(const Graphlet& g) const
        {
            //the result of the hash must be independent on the specific order you pick the string
            unsigned long val = 0;
            std::unordered_map<int, std::unordered_set<int>> repr = g.get_repr();
            std::unordered_map<int, std::unordered_set<int>>::iterator g_it = repr.begin();
            for (; g_it != repr.end(); ++g_it) {
                val = val ^ (int)((g_it->first)+(std::pow(33,3)));
            }
            
            return val;
        }
    };
}
