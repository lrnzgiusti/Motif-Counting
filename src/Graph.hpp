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
#include <list>
#include <iostream>
#include <list>
#include <numeric>
#include <random>
#include <vector>
#include <algorithm>
#include <queue>

#define MAX_NUM_NODES 10

class Graph
{
public:
    Graph();
    ~Graph();
    bool isBipartite();
    void bfs();
    std::unordered_map<int, std::list<int>> get_repr();
    void printGraph();
    
private:
    void generate_random_graph();
    void generate_random_non_bipartite_graph();
    std::unordered_map<int, std::list<int>> repr;
    uint8_t num_of_nodes;
    uint32_t num_of_edges;
    
};

#endif /* Graph_hpp */
