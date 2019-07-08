//
//  main.cpp
//  src
//
//  Created by Lorenzo Giusti on 05/04/2019.
//  Copyright © 2019 Lorenzo Giusti. All rights reserved.
//

#include "Estimator.cpp"
#include <unordered_map>
#include <algorithm>
#include <sstream>
#include <string>
#include "Utility.hpp"


#include <random>
#include <vector>


int main(int argc, char* argv[])
{
	
	
	//TODOs:
	//1- cfg file in order to provide all the info to the sampler
		//starting node,
		//how many chains,
		//value of k, how many steps,
		//how many steps for sampling a graphlets
	
	//2- multithread the chains
		//each chain runs in a separate thread, the results are probably stored into hashmap with chain is as key and objcet as value
	
	//3- choose a way to show the results of a single chain
	
	srand(time(NULL));
	
	std::unordered_map<std::string, std::string> cfg = read_config_file(argv[1]);
	
	
	
	unsigned int start = std::stoi(cfg["start"]); //starting vertex, we'll use this for create the first graphlet
	unsigned int k = std::stoi(cfg["k"]); //the number of verteces of our graphlet in the current sampling procedure
	unsigned int max_iter = std::stoi(cfg["max_iter"]); //max number of sampling step
	unsigned int lock = std::stoi(cfg["lock"]); //how many steps before sampling
	std::string data_path = cfg["data_path"]; //path of the graph in ascii format
	Graph G(data_path);
	Estimator e;
	//assert(not G.isBipartite());
	
	std::unordered_map<std::string, float> motif_distro = e.sampler_test(G, start, k, max_iter, lock);

	std::cout << std::endl;
	for (auto kv : motif_distro){
		std::cout << kv.first << "\t" << kv.second << "\n";
	}

}




