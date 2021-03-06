//
//  Graphlet.hpp
//  dummy
//
//  Created by Lorenzo Giusti on 09/04/2019.
//  Copyright © 2019 Lorenzo Giusti. All rights reserved.
//

#ifndef Graphlet_hpp
#define Graphlet_hpp


#include "Graph.cpp"
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <set>
#include <iostream>


class Graphlet: public Graph {
public:
    Graphlet(); //default constructor --probably useless
    Graphlet(std::set<std::pair<int, int>>); //get the graphlet from edgelist
    ~Graphlet();
    
    bool exclude_include_vertex(Graph  &, int, int); //exclude a vertex <int> from itself and include another vertex represented by the edge in the main graph.
   
    bool exclude_vertex(int);
    
    bool include_vertex(Graph &, int);
    
    std::unordered_map<int, std::unordered_set<int>>::iterator begin();
    std::unordered_map<int, std::unordered_set<int>>::iterator end();
    bool operator == (const Graphlet &other) const; //comparison, for make graphlet collections
    
    
private:
    int k;
};

#endif /* Graphlet_hpp */
