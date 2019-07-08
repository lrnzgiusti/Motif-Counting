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


int main(int argc, char* argv[])
{
	
	
	
	//2- multithread the chains
		//each chain runs in a separate thread, the results are probably stored into hashmap with chain is as key and objcet as value
	
	std::string data_path = argv[1]; //argv[1] contains the path of the graph.
	Graph G(data_path);
	std::map<int, std::set<int>> fast_G = graph_converter(G);
	
	
	std::vector<std::thread> threads;
	std::unordered_map<std::string, std::string> cfg;
	for(int i = 2; i < argc; i++){
		std::cout << argv[i] << "\n";
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




