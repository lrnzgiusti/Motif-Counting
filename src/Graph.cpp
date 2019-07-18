//
//  Graph.cpp
//  src
//
//  Created by Lorenzo Giusti on 05/04/2019.
//  Copyright © 2019 Lorenzo Giusti. All rights reserved.
//

#include "Graph.hpp"
#include <chrono>
using namespace std::chrono;
Graph::Graph(){
}

Graph::Graph(std::string filename){
    read_edgelist(filename);
}

Graph::Graph(std::set<std::pair<int, int>> edges){
    for(auto edge : edges){
        repr[edge.first].insert(edge.second);
        repr[edge.second].insert(edge.first);
    }
}



Graph::~Graph(){}

void Graph::read_edgelist(std::string filename){
    std::ifstream fptr;
    std::unordered_map<int, std::unordered_set<int>> G;
    fptr.open(filename);
    
    if(!fptr){
        std::cerr << "Unable to open edgelist!\tTry Again!\n";
        return;
    }
    
    std::string edgelist_row;
    std::vector<std::string> tokens;
    unsigned int tok1 = 0, tok2 = 0;
    while(std::getline(fptr, edgelist_row)){
        boost::split(tokens, edgelist_row, [](char c){return c == ',';});
        tok1 = std::stoi(tokens[0]);
        tok2 = std::stoi(tokens[1]);
        
        G[tok1].insert(tok2);
        G[tok2].insert(tok1);
        tokens.clear();
    }
    fptr.close();
    
    source = tok1;
    repr = G;
    
    return;
}


std::unordered_map<int, std::unordered_set<int>> Graph::get_repr() const{
    return repr;
}

void Graph::printGraph(){
    std::unordered_map<int, std::unordered_set<int>>::iterator it;
    for(it = repr.begin(); it != repr.end(); ++it){
        std::cout << "[" << it->first << "] -> ";
        
        std::unordered_set<int>::iterator set_iter;
        for (set_iter = it->second.begin(); set_iter != it->second.end(); ++set_iter) {
            std::cout << *set_iter << "<->";
        }
        std::cout << "NULL\n";
    }
    return;
}


bool Graph::isBipartite(){
    
    int source = source;
    size_t n_of_verteces = repr.size();
    std::unordered_map<int,int> colorArr;
    for (int i = 1; i < n_of_verteces+1; ++i)
        colorArr[i] = -1;
    
    // Assign first color to source
    colorArr[source] = 1;
    // Create a queue (FIFO) of vertex
    // numbers and enqueue source vertex
    // for BFS traversal
    std::queue <int> q;
    q.push(source);
    // Run while there are vertices
    // in queue (Similar to BFS)
    while (!q.empty())
    {
        // Dequeue a vertex from queue
        int u = q.front();
        q.pop();
        // Find all non-colored adjacent vertices
        std::unordered_set<int>::iterator it = repr[u].begin();
        
        for (; it != (repr[u].end()); ++it)
        {
            // An edge from u to v exists and
            // destination v is not colored
            if (colorArr[*it] == -1)
            {
                // Assign alternate color to this adjacent v of u
                colorArr[*it] = 1;
                q.push(*it);
            }
            // An edge from u to v exists and destination
            // v is colored with same color as u
            else if (colorArr[*it] == colorArr[u]){
                std::cout << "crossing on (" << *it << ", " << u << ")" << std::endl;
                return false;
            }
        }
    }
    // If we reach here, then all adjacent
    // vertices can be colored with alternate color
    return true;
}

// this takes 15 microsecs
bool Graph::isConnected(){
    
    unsigned int source = this->source;
    const size_t n_of_verteces = repr.size();
    std::unordered_set<int> expected_veteces;
    std::unordered_map<int, int> colorArr;
    expected_veteces.reserve(n_of_verteces);
    
    for(const std::pair<int, std::unordered_set<int>> &x: repr){
        
        expected_veteces.insert(x.first);
        colorArr[x.first] = 0;
    }
    
    //reserve expected_verteces, this->repr.size()
    //calculate expected_verteced
    std::unordered_set<int> real_veteces;

    // Assign first color to source
    colorArr[source] = 1;
    std::queue <int> q;
    q.push(source);
    real_veteces.insert(source);
    // Run while there are vertices
    // in queue (Similar to BFS)
    std::unordered_set<int>::iterator it;
    int u;

    while (!q.empty())
    {
        // Dequeue a vertex from queue
        u = q.front();
        q.pop();
        // Find all non-colored adjacent vertices
        it = repr[u].begin();
        
        for (; it != repr[u].end(); it++)
        {
            // An edge from u to v exists and
            // destination v is not colored
            if (colorArr[*it] == 0)
            {
                // Assign alternate color to this adjacent v of u
                colorArr[*it] = 1;
                q.push(*it);
                real_veteces.insert(*it);
            }
        }
    }
    return expected_veteces == real_veteces;
}


std::unordered_set<int> Graph::operator[](int idx) const {
    return repr.at(idx);
}



bool Graph::isValid() const{
    return repr.size() != 0;
}


bool Graph::exist_vertex(const int& k){
    return repr.find(k) != repr.end();
}

