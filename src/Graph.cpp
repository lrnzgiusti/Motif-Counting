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
    num_of_edges = 0;
    num_of_nodes = 0;
    for(auto edge : edges){
        repr[edge.first].insert(edge.second);
        repr[edge.second].insert(edge.first);
        num_of_edges++;
    }
    num_of_nodes = repr.size();
}

Graph::Graph(int min_degree, int max_degree, int num_of_verteces){
    num_of_edges = 0;
    num_of_nodes = 0;
    random_non_bipartite_generator(min_degree, max_degree, num_of_verteces);
    
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
    num_of_edges = 0;
    num_of_nodes = 0;
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


uint8_t Graph::get_num_of_nodes(){
    return num_of_nodes;
}

uint32_t Graph::get_num_of_edges(){
    return num_of_edges;
}

float get_density(int n_edges, int n_verteces){
    return (2.0*n_edges)/(n_verteces*(n_verteces-1));
}

void Graph::random_non_bipartite_generator(int min_degree, int max_degree, int number_of_verteces){
    
    if((min_degree > max_degree) or (max_degree > number_of_verteces)){
        std::cerr << "Parameters not valid!";
        return;
    }
    
    std::unordered_map<int, std::unordered_set<int>> G;
    int random_number_of_neighbours;
    int random_neighbor;
    for(int i = 1; i < number_of_verteces; i++){
        random_number_of_neighbours = rand()%max_degree+min_degree;
        for(int j = 0; j < random_number_of_neighbours; j++){
            random_neighbor = rand()%number_of_verteces+1;
            while(random_neighbor == i) random_neighbor = rand()%number_of_verteces+1;
            if (G[i].count(random_neighbor) == 0) {
                G[i].insert(random_neighbor);
                G[random_neighbor].insert(i);
                num_of_edges++;
            }
        }
        
    }
    this->repr = G;
    this->num_of_nodes = G.size();
    return;
}



std::vector<float> Graph::random_walk(){
    std::vector<float> distro_t(num_of_nodes+1);
    std::vector<float> distro_tprec(num_of_nodes+1);
    float epsilon = 1e-6;
    int t = 1;
    distro_t[1] = 1;
    int current_vertex = 1;
    auto it = std::next(repr[current_vertex].begin(), rand()%repr[current_vertex].size()) ;
    do{
        t++;
        distro_tprec = distro_t;
        it = std::next(repr[current_vertex].begin(), rand()%repr[current_vertex].size());
        current_vertex = *it;
        distro_t[current_vertex]++;
    }while (l1_diff(distro_t, distro_tprec,t) > epsilon);
    

    for(int i = 0; i < distro_t.size(); i++){
        distro_t[i] /= t;
    }
    
    std::cout << t << "\n";
    return distro_t;
}



void Graph::random_bipartite_generator(int min_degree, int max_degree, int number_of_verteces){
    
    if((min_degree > max_degree) or (max_degree > number_of_verteces)){
        std::cerr << "Parameters not valid!";
        return;
    }
    
    int current = 1;
    int sub_current = 2;
    std::unordered_map<int, std::unordered_set<int>> G;
    int current_number_of_edges = 0;
    int color[number_of_verteces];
    color[current-1] = 0;
    int n_of_neigh;
    int first_random_vertex, second_random_vertex;
    
    while (G.size() <= number_of_verteces) {
        n_of_neigh = rand()%max_degree+min_degree;
        for (int i = 0; i < n_of_neigh; ++i, ++sub_current) {
            G[current].insert(sub_current);
            G[sub_current].insert(current);
            current_number_of_edges++;
            color[sub_current-1] = !color[current-1];
            
            //the first condition tells us to avoid insert edges if it's unecessary.
            // and avoid to compute the density with less then two verteces.
            //refill the graph until you get an acceptable density
            while ( (G.size() > 2) and (G.size() <= number_of_verteces) and get_density(current_number_of_edges, (int)G.size()) < .15) {
                first_random_vertex = rand()%sub_current+1;
                second_random_vertex = rand()%sub_current+1;
                
                //if you pick the same colors you end up with a non-bipartite graph.
                while (color[second_random_vertex-1] == color[first_random_vertex-1]){
                    first_random_vertex = rand()%sub_current+1;
                    second_random_vertex = rand()%sub_current+1;
                }
                
                //classical connection between two verteces.
                G[first_random_vertex].insert(second_random_vertex);
                G[second_random_vertex].insert(first_random_vertex);
                current_number_of_edges++;
            }
            current = sub_current;
        }
    }
    this->repr = G;
    return;
}
