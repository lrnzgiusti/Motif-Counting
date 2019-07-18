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
#include <string>
#include <vector>
#include <thread>




std::map<int, std::set<int>> graph_converter(Graph &G){
	std::map<int, std::set<int>> fast_G;
	for(const std::pair<int, std::unordered_set<int>> &v : G.get_repr()){
		for(const int &nv : G[v.first]){
			fast_G[v.first].insert(nv);
		}
	}
	return fast_G;
}


std::map<int, std::set<int>> read_edgelist(std::string filename){
	std::ifstream fptr;
	std::map<int, std::set<int>> G;
	fptr.open(filename);
	
	if(!fptr){
		std::cerr << "Unable to open edgelist!\tTry Again!\n";
		return G;
	}
	std::cout << "File opened successfully\n";
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
	std::cout << "Graph loaded!\n";
	std::cout << G.size() << "\n" ;
	return G;
}




std::vector<std::set<int>> read_edgelist_v(std::string filename, size_t n){
	std::ifstream fptr;
	//std::vector<std::set<int>> G;
	//G.reserve(63731);
	
	std::vector<std::set<int>> G(n);
	fptr.open(filename);
	
	if(!fptr){
		std::cerr << "Unable to open edgelist!\tTry Again!\n";
		return G;
	}
	std::cout << "File opened successfully\n";
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
	std::cout << "Graph loaded!\n";
	return G;
}


int main(int argc, char* argv[])
{
	
	/***
	  TRY an implementation with a vector
	 **/
	std::string data_path = argv[1]; //argv[1] contains the path of the graph.
	//Graph G(data_path);
	std::map<int, std::set<int>> fast_G = read_edgelist(data_path);
	std::vector<std::set<int>> G = read_edgelist_v(data_path, 63731);
	for(const int &n : G[0]){
		std::cout << n  << "\n";
	}
	return 0;
	std::vector<std::thread> threads;
	std::unordered_map<std::string, std::string> cfg;
	for(int i = 2; i < argc; i++){
		cfg = read_config_file(argv[i]);
		threads.push_back(std::thread([&cfg, &fast_G]{
			unsigned int start = std::stoi(cfg["start"]); //starting vertex, we'll use this for create the first graphlet
			unsigned int k = std::stoi(cfg["k"]); //the number of verteces of our graphlet in the current sampling procedure
			unsigned int max_iter = std::stoi(cfg["max_iter"]); //max number of sampling step
			unsigned int lock = std::stoi(cfg["lock"]); //how many steps before sampling
			unsigned long seed = std::stoi(cfg["seed"]);
			srand(seed);
			
			Estimator e;
			
			
			e.sampler_test(fast_G, start, k, max_iter, lock);
		}));
	}
	
	for (std::thread & th : threads)
	{
		// If thread Object is Joinable then Join that thread.
		if (th.joinable())
			th.join();
	}
	
	return 0;


}




