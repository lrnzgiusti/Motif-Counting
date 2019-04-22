//
//  main.cpp
//  src
//
//  Created by Lorenzo Giusti on 05/04/2019.
//  Copyright © 2019 Lorenzo Giusti. All rights reserved.
//


// TODO: random edge placement
// TODO: read edgelist
#include "Estimator.cpp"


#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include "Utility.hpp"


int main(int argc, char* argv[])
{
	/*
    Graph g = *new Graph("/Users/ince/Desktop/edgelist.csv");
    g.printGraph();
	std::cout << "Test Bipartition: " << (g.isBipartite(1) ? "Positive\n" : "Negative\n");
	std::cout << "Test Connection: " << (g.isConnected(1) ? "Positive\n" : "Negative\n");
	std::cout << "\n" <<"\n";
	*/
	
	
	srand(time(NULL));
	/*
	Graph random_graph(1,2,8);
	random_graph.printGraph();
	std::cout << "Test Bipartition: " << (random_graph.isBipartite(1) ? "Positive\n" : "Negative\n");
	std::cout << "Test Connection: " << (random_graph.isConnected(1) ? "Positive\n" : "Negative\n");
	std::cout << "\n" <<"\n";
	*/
	
	/*
	std::vector<float> vec = random_graph.random_walk();
	for(int i = 1; i < vec.size(); i++) std::cout<<"index: " << i << " Value: "<< vec[i] << "\n";
	std::cout << "\n";
	std::cout << "Number of edges: " << random_graph.get_num_of_edges() << "\n";
	*/
	
	Graph G("/Users/ince/Desktop/edgelist.csv");
	G.printGraph();
	
	std::unordered_map<Graphlet, std::unordered_set<Graphlet>> Gk = Estimator().random_walk_test(G, 1, 3);
	for(std::pair<Graphlet, std::unordered_set<Graphlet>> g_p : Gk){
		g_p.first.printGraph();
		std::cout << "-----##---##--##--##\n";
		for(Graphlet g : g_p.second){
			std::cout << "\n";
			g.printGraph();
			std::cout << "\n";
		}
		std::cout << "---------------\n";
	}
}




