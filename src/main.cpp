//
//  main.cpp
//  src
//
//  Created by Lorenzo Giusti on 05/04/2019.
//  Copyright © 2019 Lorenzo Giusti. All rights reserved.
//

#include "Estimator.cpp"
#include <unordered_map>
#include "Utility.hpp"
#include "Aggregator.cpp"
#include <string>
#include <limits.h>



Graph read_edgelist(std::vector<std::string> edgelist){
	unsigned int num_of_nodes = 0;
	unsigned int num_of_edges = 0;
	unsigned int min_degree = INT_MAX;
	unsigned int max_degree = 0;
	
	std::map<int, std::set<int>> adj_list;
	
	std::vector<std::string> tokens;
	unsigned int tok1 = 0, tok2 = 0;
	for (std::string s : edgelist){
		boost::split(tokens, s, [](char c){return c == ',';});
		tok1 = std::stoi(tokens[0]);
		tok2 = std::stoi(tokens[1]);
		
		adj_list[tok1].insert(tok2);
		adj_list[tok2].insert(tok1);
		num_of_edges++;
		max_degree = (adj_list[tok1].size() > max_degree) ? (unsigned int)adj_list[tok1].size() : max_degree;
		min_degree = (adj_list[tok1].size() < min_degree) ? (unsigned int)adj_list[tok1].size() : min_degree;
		tokens.clear();
	}
	num_of_nodes = (unsigned int)adj_list.size();
	
	std::cout << "Graph loaded!\n" << "Number of nodes: " << num_of_nodes
			  << "\nNumber of edges: " << num_of_edges
			  << "\nMax degree: " << max_degree
			  << "\nMin degree: " << min_degree << "\n";
	Graph G(adj_list);
	G.num_of_nodes = num_of_nodes;
	G.num_of_edges = num_of_edges;
	G.max_degree = max_degree;
	G.min_degree = min_degree;
	return G;
}




int main(int argc, char* argv[])
{
	
	std::string data_path = argv[1]; //argv[1] contains the path of the graph.
	
	//this load the edgelist file into the memory, it's the fastest way to read a text file
	std::string s(std::istreambuf_iterator<char>(std::ifstream(data_path)
												 >> std::skipws),
				  std::istreambuf_iterator<char>());
	
	
	std::vector<std::string> tokens;
	//put the edges into a vector, this will be passed to the function that builds up the graph
	boost::split(tokens, s, [](char c){return c == '\n';});
	
	Graph fast_G = read_edgelist(tokens); //build the graph
	
	std::unordered_map<std::string, std::string> cfg = read_config_file(argv[2]); //read the configuration file
	
	unsigned int start = std::stoi(cfg["start"]); //starting vertex, we'll use this for create the first graphlet
	unsigned int k = std::stoi(cfg["k"]); //the number of verteces of our graphlet in the current sampling procedure
	unsigned int max_iter = std::stoi(cfg["max_iter"]); //max number of sampling step
	unsigned int lock = std::stoi(cfg["lock"]); //how many steps before sampling
	unsigned long seed = std::stoi(cfg["seed"]);
	srand(seed);
	
	
	
	Estimator e;
	e.sampler_weighted(fast_G, start, k, max_iter, lock);

	return 0;
}




