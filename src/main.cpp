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


Graph read_edgelist(std::vector<std::string> &edgelist){
	unsigned int max_degree = 0;
	
	Graph G;

	std::vector<std::string> tokens;
	unsigned int tok1 = 0, tok2 = 0;
	for (std::string s : edgelist){
		try{
			boost::split(tokens, s, [](char c){return c == ',';});
			tok1 = std::stoi(tokens[0]);
			tok2 = std::stoi(tokens[1]);
			
			G.insertEdge(tok1, tok2);

			tokens.clear();
		}
		catch(...){
			std::cout << "Catch!";
		}
	}

	std::cout << "Graph loaded!" << "\n";

	return G;
}




int main(int argc, char* argv[])
{
	std::string data_path = argv[1]; //argv[1] contains the path of the graph.
	
	//this load the edgelist file into the memory, it's the fastest way to read a text file
	std::string *s = new std::string(std::istreambuf_iterator<char>(std::ifstream(data_path)
												 >> std::skipws),
				  std::istreambuf_iterator<char>());
	
	
	std::vector<std::string> tokens, blank;
	//put the edges into a vector, this will be passed to the function that builds up the graph
	boost::split(tokens, *s, [](char c){return c == '\n';});
	delete s;
	Graph fast_G = read_edgelist(tokens); //build the graph
	std::vector<std::string>().swap(tokens);
	std::unordered_map<std::string, std::string> cfg = read_config_file(argv[2]); //read the configuration file
	
	unsigned int start = std::stoi(cfg["start"]); //starting vertex, we'll use this for create the first graphlet
	unsigned int k = std::stoi(cfg["k"]); //the number of verteces of our graphlet in the current sampling procedure
	unsigned int max_iter = std::stoi(cfg["max_iter"]); //max number of sampling step
	unsigned int lock = std::stoi(cfg["lock"]); //how many steps before sampling
	unsigned long seed = std::stoi(cfg["seed"]);
	int sleep = std::stoi(cfg["sleep"]);
	int run = std::stoi(cfg["run"]);
	srand(seed);


	Estimator e;
	for (int i = 0; i < run; i++) {
		e.sampler_weighted(fast_G, start, k, max_iter, lock);
		std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
	}

	
	/**
	 WARNING: CHANGE k3_string_to_minhash TO k5_string_to_minhash
	 **/
	
	//perform_L1(k3_test);
	

	return 0;
}




