//
//  Graph.hpp
//  src
//
//  Created by Lorenzo Giusti on 05/04/2019.
//  Copyright © 2019 Lorenzo Giusti. All rights reserved.
//

#ifndef Graph_hpp
#define Graph_hpp


#include <unordered_map>
#include <iostream>
#include <unordered_set>
#include <numeric>
#include <random>
#include <algorithm>
#include <set>
#include <map>
#include <queue>
#include <fstream>
#include <string>
#include <boost/algorithm/string.hpp>


class Graph
{
public:
    Graph();
    Graph(std::string);
    Graph(std::map<int, std::set<int>>);
    Graph(std::set<std::pair<int, int>>);
    ~Graph();
    
    unsigned int num_of_nodes;
    unsigned int num_of_edges;
    unsigned int min_degree;
    unsigned int max_degree;
    
    bool isConnected(); //same as isBipartite
    bool isValid() const;
    const char* text_footprint() const;
    std::unordered_map<int, std::unordered_set<int>> get_repr() const;
    std::map<int, std::set<int>> get_ordered_repr() const;
    
    void printGraph();
    bool exist_vertex(const int& k);
    void insertEdge(int, int);
    
    std::set<int>*get(int);
    std::set<int> operator[](int) const;
    inline bool has_edge(unsigned int i, unsigned int j) const //FIXME: Could be invoked with j>=i
    {
        
        try {
            return repr.at(i).find(j) != repr.at(i).end();
        } catch (...) {
            return false;
        }
        
    }
    
    unsigned int get_size() const
    {
        return static_cast<unsigned int>(repr.size());
    }
    
    
private:
    void read_edgelist(std::string);
    
    
    
protected:
    std::unordered_map<int, std::unordered_set<int>> repr;
    std::map<int, std::set<int>> ordered_repr;
    unsigned int source;
};

std::ostream& operator<<(std::ostream& os, const Graph& G)
{
    os << "[ ";
    for(std::pair<int, std::unordered_set<int>> p : G.get_repr()){
        os << p.first << " " ;
    }
    os << "]";
    return os;
}

#endif /* Graph_hpp */
