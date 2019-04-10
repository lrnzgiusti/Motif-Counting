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
    read_edgelist("edgelist.csv");
}

Graph::Graph(std::unordered_map<int, std::unordered_set<int>> repr){
    this->repr = repr;
}

Graph::~Graph(){}

//the graph becomes std::unordered_map<int, std::unordered_set<int>>
std::unordered_set<int> random_neighbourhood(int avoid){
    //the pool you pick randomly the connected edges.
    std::vector<int> pool (MAX_NUM_NODES);
    //fill the pool with increasing numbers until you reach the end of the vector
    std::iota(pool.begin(), pool.end(), 1);
    //remove self loops
    pool.erase(std::remove(pool.begin(), pool.end(), avoid), pool.end());
    std::unordered_set<int> rng_set;
    int k; //the random index you pick from the pool
    int max_neighbours = rand()%(MAX_NUM_NODES/2)+2;
    for(int i = 0; i < max_neighbours; ++i){
        if(pool.size() == 0) return rng_set; //if you don't have element to pick return the list, maybe obsolete
        k = rand()%pool.size();
        rng_set.insert(pool[k]);
        pool.erase(pool.begin()+k);
    }
    
    return rng_set;
}

void fix_undirection(std::unordered_map<int, std::unordered_set<int>> &G){
    std::unordered_map<int, std::unordered_set<int>>::iterator it;
    for(it = G.begin(); it != G.end(); ++it){
        std::unordered_set<int>::iterator set_iter;
        for (set_iter = it->second.begin(); set_iter != it->second.end(); ++set_iter) {
            G[*set_iter].insert(it->first);
            //G.num_of_edges+=G[*set_iter].size(); non ho accesso al metodo privato
        }
    }
    return;
}


void Graph::random_undirected_edge_placement(){
    std::unordered_map<int, std::unordered_set<int>> G;
    for (int i = 1; i < MAX_NUM_NODES+1; ++i) {
        int edges = rand()%MAX_NUM_NODES/2+1;
        for (int j = 1; j < edges; ++j) {
            int k = rand()%MAX_NUM_NODES+1;
            while (k == i) {
                k = rand()%MAX_NUM_NODES+1;
            }
            G[i].insert(k);
            G[k].insert(i);
            this->num_of_edges++;
            this->num_of_nodes++;
        }
    }
    this->repr = G;
    return;
}


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
    this->repr = G;
    return;
}

void Graph::generate_random_graph(){
    std::unordered_map<int, std::unordered_set<int>> G;
    for(int i = 1; i < MAX_NUM_NODES+1; ++i){
        G[i] = random_neighbourhood(i);
        this->num_of_nodes++;
    }
    //since after the previous procedure the graph is directed,
    //we have to fix by inserting the respective couter-edge
    fix_undirection(G);
    this->repr = G;
    return;
}

void Graph::generate_random_non_bipartite_graph(){
    this->random_undirected_edge_placement();
    while (this->isBipartite()) {
        this->random_undirected_edge_placement();
    }
    return;
}



std::unordered_map<int, std::unordered_set<int>> Graph::get_repr(){
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
                return false;
            }
        }
    }
    // If we reach here, then all adjacent
    // vertices can be colored with alternate color
    return true;
}
