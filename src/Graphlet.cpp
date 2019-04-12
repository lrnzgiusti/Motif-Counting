//
//  Graphlet.cpp
//  dummy
//
//  Created by Lorenzo Giusti on 09/04/2019.
//  Copyright © 2019 Lorenzo Giusti. All rights reserved.
//

#include "Graphlet.hpp"

Graphlet::Graphlet(){}

Graphlet::Graphlet(std::unordered_map<int, std::unordered_set<int>> repr) : Graph(repr){}

Graphlet::Graphlet(std::set<std::pair<int, int>> edges) : Graph(edges){}

Graphlet::~Graphlet(){}

bool Graphlet::operator ==(const Graphlet &g) const{
    std::unordered_map<int, std::unordered_set<int>> other_graph_representation = g.get_repr();
    if (num_of_edges != g.num_of_edges) return false;
    if (num_of_nodes != g.num_of_nodes) return false;
    for (std::pair<int, std::unordered_set<int>> p : other_graph_representation) {
        try {
            if (repr.at(p.first) != other_graph_representation.at(p.first)) return false;
        } catch (...) {
            return false;
        }
    }
    return true;
}

namespace std {
    // we'll use this for make sets and maps of graphlets, needed for GRASP patterns
    template <>
    struct hash<Graphlet>
    {
        std::size_t operator()(const Graphlet& g) const
        {
            string s = "";
            std::unordered_map<int, std::unordered_set<int>> repr = g.get_repr();
            std::unordered_map<int, std::unordered_set<int>>::iterator g_it = repr.begin();
            for (; g_it != repr.end(); ++g_it) {
                s += to_string(g_it->first);
            }
            return hash<string>()(s);
        }
    };
    
}
