//
//  Graph.cpp
//  src
//
//  Created by Lorenzo Giusti on 05/04/2019.
//  Copyright © 2019 Lorenzo Giusti. All rights reserved.
//

#include "Graph.hpp"

Graph::Graph(){}

Graph::Graph(std::string filename){
    num_of_nodes = 0;
    num_of_edges = 0;
    read_edgelist(filename);
}

Graph::Graph(std::unordered_map<int, std::unordered_set<int>> repr){
    this->repr = repr;
    this->num_of_nodes = this->repr.size();
    
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
    while(fptr >> edgelist_row){
        boost::split(tokens, edgelist_row, [](char c){return c == ',';});
        int tok1 = std::stoi(tokens[0]);
        int tok2 = std::stoi(tokens[1]);
        
        G[tok1].insert(tok2);
        G[tok2].insert(tok1);
        tokens.clear();
        this->num_of_edges++;
    }
    fptr.close();
    this->num_of_nodes = G.size();
    this->repr = G;
    return;
}




std::unordered_map<int, std::unordered_set<int>> Graph::get_repr() const{
    return this->repr;
}

void Graph::printGraph(){
    std::unordered_map<int, std::unordered_set<int>>::iterator it;
    for(it = this->repr.begin(); it != this->repr.end(); ++it){
        std::cout << "[" << it->first << "] -> ";
        
        std::unordered_set<int>::iterator set_iter;
        for (set_iter = it->second.begin(); set_iter != it->second.end(); ++set_iter) {
            std::cout << *set_iter << "<->";
        }
        std::cout << "NULL\n";
    }
    return;
}


bool Graph::isBipartite(int source){
    size_t n_of_verteces = this->repr.size();
    int colorArr[n_of_verteces+1];
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
        std::unordered_set<int>::iterator it = this->repr[u].begin();
        
        for (; it != (this->repr[u].end()); ++it)
        {
            // An edge from u to v exists and
            // destination v is not colored
            if (colorArr[*it] == -1)
            {
                // Assign alternate color to this adjacent v of u
                colorArr[*it] = 1 - colorArr[u];
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


bool Graph::isConnected(int source){
    size_t n_of_verteces = this->repr.size();
    std::unordered_set<int> expected_veteces;
    expected_veteces.reserve(n_of_verteces);
    for(auto x: this->repr){
        expected_veteces.insert(x.first);
    }
    //reserve expected_verteces, this->repr.size()
    //calculate expected_verteced
    std::unordered_set<int> real_veteces;
    //reserve expected_verteces, this->repr.size()
    
    
    int colorArr[n_of_verteces+1]; //instead of max_num_nodes we have to insert the dimension of the graph
    for (int i = 1; i < n_of_verteces+1; ++i)
        colorArr[i] = -1;
    
    // Assign first color to source
    colorArr[source] = 1;
    std::queue <int> q;
    q.push(source);
    real_veteces.insert(source);
    // Run while there are vertices
    // in queue (Similar to BFS)
    while (!q.empty())
    {
        // Dequeue a vertex from queue
        int u = q.front();
        q.pop();
        // Find all non-colored adjacent vertices
        std::unordered_set<int>::iterator it = this->repr[u].begin();
        
        for (; it != (this->repr[u].end()); ++it)
        {
            // An edge from u to v exists and
            // destination v is not colored
            if (colorArr[*it] == -1)
            {
                // Assign alternate color to this adjacent v of u
                colorArr[*it] = 1 - colorArr[u];
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
