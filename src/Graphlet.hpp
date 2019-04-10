//
//  Graphlet.hpp
//  dummy
//
//  Created by Lorenzo Giusti on 09/04/2019.
//  Copyright © 2019 Lorenzo Giusti. All rights reserved.
//

#ifndef Graphlet_hpp
#define Graphlet_hpp

#include <stdio.h>
#include <unordered_map>
#include <unordered_set>
class Graphlet{
public:
    Graphlet(); //default constructor --probably useless
    Graphlet(std::unordered_map<int, std::unordered_set<int>>); //get the graphlet from an adjacency list
    ~Graphlet();
    std::unordered_map<int, std::unordered_set<int>> get_repr() const;
    bool operator == (const Graphlet &other) const; //comparison, for make graphlet collections
    
private:
    int k;
    std::unordered_map<int, std::unordered_set<int>> repr;
};

#endif /* Graphlet_hpp */
