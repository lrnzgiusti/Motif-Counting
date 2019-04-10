//
//  Graphlet.cpp
//  dummy
//
//  Created by Lorenzo Giusti on 09/04/2019.
//  Copyright © 2019 Lorenzo Giusti. All rights reserved.
//

#include "Graphlet.hpp"
#include <string>

Graphlet::Graphlet(){}

Graphlet::Graphlet(std::unordered_map<int, std::unordered_set<int>> repr){
    this->repr = repr;
}

Graphlet::~Graphlet(){}


std::unordered_map<int, std::unordered_set<int>> Graphlet::get_repr() const {
    return this->repr;
}


bool Graphlet::operator ==(const Graphlet &g) const{
    std::unordered_map<int, std::unordered_set<int>> repr = g.get_repr();
    std::unordered_map<int, std::unordered_set<int>>::iterator g_it = repr.begin();
    for (; g_it != repr.end(); ++g_it) {
        if (this->repr.at(g_it->first) != repr.at(g_it->first)) {
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
