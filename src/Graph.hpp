//
//  Graph.hpp
//  src
//
//  Created by Lorenzo Giusti on 05/04/2019.
//  Copyright © 2019 Lorenzo Giusti. All rights reserved.
//

#ifndef Graph_hpp
#define Graph_hpp


#include <vector>
#include <unordered_map>
#include <iostream>
#include <unordered_set>
#include <numeric>
#include <random>
#include <algorithm>
#include <queue>
#include <fstream>
#include <string>
#include <boost/algorithm/string.hpp>
#include "Utility.hpp"


class Graph
{
public:
    Graph();
    Graph(std::string);
    Graph(std::unordered_map<int, std::unordered_set<int>>);
    Graph(std::set<std::pair<int, int>>);
    Graph(int, int, int);
    ~Graph();
    bool isBipartite(); //the formal parameter is the source vertex in which you start the traversal
    bool isConnected(); //same as isBipartite
    bool isUndirected(int);
    uint8_t get_num_of_nodes();
    uint32_t get_num_of_edges();
    
    std::unordered_map<int, std::unordered_set<int>> get_repr() const;
    std::vector<float> random_walk();
    void printGraph();
    std::unordered_set<int> operator[](int) const;
    
private:
    void read_edgelist(std::string);
    
protected:
    std::unordered_map<int, std::unordered_set<int>> repr;
    uint8_t num_of_nodes;
    uint32_t num_of_edges;
    uint32_t max_degree;
    uint32_t min_degree;
    void random_bipartite_generator(int, int, int);
    void random_non_bipartite_generator(int, int, int);
};

std::ostream& operator<<(std::ostream& os, const Graph& G)
{
    for(std::pair<int, std::unordered_set<int>> p : G.get_repr()){
        os << p.first << "-" ;
    }
    os << "><";
    return os;
}

#endif /* Graph_hpp */
