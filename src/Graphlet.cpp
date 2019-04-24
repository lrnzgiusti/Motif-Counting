//
//  Graphlet.cpp
//  dummy
//
//  Created by Lorenzo Giusti on 09/04/2019.
//  Copyright © 2019 Lorenzo Giusti. All rights reserved.
//

#include "Graphlet.hpp"
#include <math.h>
#include <chrono>
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


/**
 Vanilla: Create a new graph by copying the reference to this, exclude the vertex <excl> and include the vertex <incl>
 Gamma1: since the constructor take very long time, i perform the operation on <this> and <this> is a reference to the graphlet i have to exclude-include
 */
void Graphlet::exclude_include_vertex(Graph G, int excl, int incl){
    //erase <excl> from the nodes of the graph
    this->repr.erase(excl);
    
    //erase <excl> from the neighbors
    for(std::pair<int, std::unordered_set<int>> check_edges : *this){
        this->repr[check_edges.first].erase(excl);
    }
    
    //place <incl> in the vertex and in the neighbords
    for (int vertex : G[incl]) {
        if(this->repr.find(vertex) != this->repr.end()){
            this->repr[incl].insert(vertex);
            this->repr[vertex].insert(incl);
        }
    }
    return;
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
