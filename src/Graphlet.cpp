//
//  Graphlet.cpp
//  dummy
//
//  Created by Lorenzo Giusti on 09/04/2019.
//  Copyright © 2019 Lorenzo Giusti. All rights reserved.
//

#include "Graphlet.hpp"

Graphlet::Graphlet(){}

Graphlet::Graphlet(std::set<std::pair<int, int>> edges) : Graph(edges){}

Graphlet::~Graphlet(){}

bool Graphlet::operator ==(const Graphlet &g) const{
    std::unordered_map<int, std::unordered_set<int>> other_graph_representation = g.get_repr();
    
    for (const std::pair<int, std::unordered_set<int>> &p : other_graph_representation) {
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


bool Graphlet::exclude_vertex(int excl)
{
    repr.erase(excl);
    //erase <excl> from the neighbors and add connections
    for(const std::pair<int, std::unordered_set<int>> &check_edges : *this){
        repr[check_edges.first].erase(excl);
        //this->source = check_edges.first;
    }
    source = repr.begin()->first;
    return isConnected();
}


bool Graphlet::include_vertex(std::map<int, std::set<int>> &G, int incl){
    for(const std::pair<int, std::unordered_set<int>> &check_edges : *this){
        if(G[incl].find(check_edges.first) != G[incl].end()){
            repr[incl].insert(check_edges.first);
            repr[check_edges.first].insert(incl);
        }
    }
    source = incl;
    return true;
}



bool Graphlet::exclude_include_vertex(std::map<int, std::set<int>> &G, int excl, int incl){

    //erase <excl> from the nodes of the graph
    repr.erase(excl);
    //erase <excl> from the neighbors and add connections
    for(const std::pair<int, std::unordered_set<int>> &check_edges : *this){
        repr[check_edges.first].erase(excl);
        if(G.at(incl).find(check_edges.first) != G.at(incl).end()){ 
        repr[incl].insert(check_edges.first);
        repr[check_edges.first].insert(incl);
        }
    }
    
    source = incl;
    return isConnected();
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
