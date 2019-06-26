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

Graphlet::Graphlet(std::string s){
    //string has the format [ v1 v2 v3 ] we want a graph that has no edges but the nodes, in order to hash it.
    
    std::vector<std::string> tokens;
    boost::split(tokens, s, [](char c){return c == ' ';});
    for(int i = 1; i < tokens.size()-1; i++){
        this->repr[std::stoi(tokens[i])] = {};
    }
}

Graphlet::~Graphlet(){}

bool Graphlet::operator ==(const Graphlet &g) const{
    std::unordered_map<int, std::unordered_set<int>> other_graph_representation = g.get_repr();
    
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
//this takes around 70 microsecs
bool Graphlet::exclude_include_vertex(Graph &G, int excl, int incl){
    
    //erase <excl> from the nodes of the graph
    this->repr.erase(excl);
    
    //erase <excl> from the neighbors and add connections
    
    for(std::pair<int, std::unordered_set<int>> check_edges : *this){
        this->repr[check_edges.first].erase(excl);
        if(G[incl].find(check_edges.first) != G[incl].end()){ //qua ne becca al massimo un paio TODO
            
            this->repr[incl].insert(check_edges.first);
            this->repr[check_edges.first].insert(incl);
        }
    }

    this->source = incl;
    return this->isConnected();
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
                val = val  ^ (unsigned long)(g_it->first * (unsigned long) (7*std::pow(37,7)-27) << 2) ;
                
            }
            return val;
        }
    };
}
