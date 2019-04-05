//
//  Graph.cpp
//  src
//
//  Created by Lorenzo Giusti on 05/04/2019.
//  Copyright © 2019 Lorenzo Giusti. All rights reserved.
//

#include "Graph.hpp"


Graph::Graph(){
    num_of_nodes = 0;
    num_of_edges = 0;
    generate_random_non_bipartite_graph();
}

Graph::~Graph(){}

std::list<int> random_list(int avoid){
    //the pool you pick randomly the connected edges.
    std::vector<int> pool (MAX_NUM_NODES);
    //fill the pool with increasing numbers until you reach the end of the vector
    std::iota(pool.begin(), pool.end(), 1);
    //remove self loops
    pool.erase(std::remove(pool.begin(), pool.end(), avoid), pool.end());
    std::list<int> rng_list (rand()%MAX_NUM_NODES+1, -1);
    int k; //the random index you pick from the pool
    for(std::list<int>::iterator it = rng_list.begin(); it != rng_list.end(); ++it){
        if(pool.size() == 0) return rng_list; //if you don't have element to pick return the list, maybe obsolete
        k = rand()%pool.size();
        *it = pool[k];
        pool.erase(pool.begin()+k);
    }
    
    return rng_list;
}

void Graph::generate_random_graph(){

    std::unordered_map<int, std::list<int>> G;
    for(int i = 1; i < MAX_NUM_NODES+1; ++i){
        G[i] = random_list(i);
        this->num_of_nodes++;
        this->num_of_edges+=G[i].size();
    }
    this->repr = G;
    return;
}

void Graph::generate_random_non_bipartite_graph(){
    this->generate_random_graph();
    while (this->isBipartite()) {
        this->generate_random_graph();
    }
    return;
}

std::unordered_map<int, std::list<int>> Graph::get_repr(){
    return this->repr;
}


void Graph::printGraph(){
    std::unordered_map<int, std::list<int>>::iterator it;
    for(it = this->repr.begin(); it != this->repr.end(); ++it){
        std::cout << "[" << it->first << "] -> ";
        
        std::list<int>::iterator list_iter;
        for (list_iter = it->second.begin(); list_iter != it->second.end(); ++list_iter) {
            std::cout << *list_iter << "<->";
        }
        std::cout << "NULL\n";
    }
    return;
}

void Graph::bfs()
{
    int source = 1;
    int visited[MAX_NUM_NODES+1];
    for (int i = 1; i < MAX_NUM_NODES+1; ++i)
        visited[i] = -1;
    std::queue <int> q;
    q.push(source);
    visited[source] = 1;
    while (!q.empty())
    {
        // Dequeue a vertex from queue
        int u = q.front();
        q.pop();
        
        std::cout << "Visiting " << u << "\n";
        
        
        // Find all non-colored adjacent vertices
        std::list<int>::iterator it = this->repr[u].begin();
        
        for (; it != (this->repr[u].end()); ++it)
        {
            
            // An edge from u to v exists and
            // destination v is not colored
            if (visited[*it] == -1)
            {
                // Assign alternate color to this adjacent v of u
                visited[*it] = 1;
                q.push(*it);
                
            }
        }
    }
    
}

bool Graph::isBipartite(){
    int source = 1;
    int colorArr[MAX_NUM_NODES+1];
    for (int i = 1; i < MAX_NUM_NODES+1; ++i)
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
        std::list<int>::iterator it = this->repr[u].begin();
        
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
                std::cout << "Crossing on: (" <<*it << "," << u << ")\n";
                return false;
            }
        }
    }
    
    // If we reach here, then all adjacent
    // vertices can be colored with alternate color
    return true;
}
