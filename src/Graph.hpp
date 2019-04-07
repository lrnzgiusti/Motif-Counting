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


#define MAX_NUM_NODES 10

class Graph
{
public:
    Graph();
    ~Graph();
    bool isBipartite();
    std::unordered_map<int, std::unordered_set<int>> get_repr();
    void printGraph();
    
private:
    void generate_random_graph();
    void random_undirected_edge_placement();
    void generate_random_non_bipartite_graph();
    void read_edgelist(std::string);
    std::unordered_map<int, std::unordered_set<int>> repr;
    uint8_t num_of_nodes;
    uint32_t num_of_edges;
    
};

#endif /* Graph_hpp */
