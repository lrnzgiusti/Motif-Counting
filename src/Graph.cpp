//
//  Graph.cpp
//  src
//
//  Created by Lorenzo Giusti on 05/04/2019.
//  Copyright © 2019 Lorenzo Giusti. All rights reserved.
//

#include "Graph.hpp"
Graph::Graph(){
}

Graph::Graph(std::map<int, std::set<int>> adj_list){
    ordered_repr = adj_list;
}

Graph::Graph(std::string filename){
   
}

Graph::Graph(std::set<std::pair<int, int>> edges){
    for(auto edge : edges){
        repr[edge.first].insert(edge.second);
        repr[edge.second].insert(edge.first);
    }
}



Graph::~Graph(){}

std::unordered_map<int, std::unordered_set<int>> Graph::get_repr() const{
    return repr;
}

std::map<int, std::set<int>> Graph::get_ordered_repr() const{
    return ordered_repr;
}

void Graph::printGraph(){
     std::map<int, std::set<int>>::iterator it;
    	for(it = ordered_repr.begin(); it != ordered_repr.end(); ++it){
		std::cout << "[" << it->first << "] -> ";
	
		std::set<int>::iterator set_iter;
	for (set_iter = it->second.begin(); set_iter != it->second.end(); ++set_iter) {
	    std::cout << *set_iter << "<->";
	}
	std::cout << "NULL\n";

}
    return;
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
    return expected_veteces == real_veteces;;
}


std::set<int>* Graph::get(int idx){
    return &ordered_repr[idx];
}

std::set<int> Graph::operator[](int idx) const {
    return ordered_repr.at(idx);
}

void Graph::insertEdge(int u, int v){
    ordered_repr[u].insert(v);
    ordered_repr[v].insert(u);
}

bool Graph::isValid() const{
    return repr.size() != 0;
}


bool Graph::exist_vertex(const int& k){
    return repr.find(k) != repr.end();
}

