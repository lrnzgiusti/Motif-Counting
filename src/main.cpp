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
	
	
	Graph G("/Users/ince/Desktop/edgelist.csv");
	G.printGraph();
	std::cout << "\n";
	std::unordered_map<Graphlet, std::unordered_set<Graphlet>> Gk = Estimator().random_walk(G, 1, 4);

	//TODO: Fare un toString del graphlet e vedere se i nodi mancanti vengono presi in considerazione almeno una volta
	
	for(std::pair<Graphlet, std::unordered_set<Graphlet>> g_p : Gk){
		g_p.first.printGraph();
		std::cout << "-----##---##--##--##\n";
		for(Graphlet g : g_p.second){
			std::cout << "\n";
			g.printGraph();
			std::cout << "\n";
		}
		std::cout << "---------------\n";
		std::cout << "---------------\n";
		std::cout << "---------------\n";
	}
}




