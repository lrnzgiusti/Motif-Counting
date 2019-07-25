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



std::map<int, std::set<int>> read_edgelist(std::vector<std::string> edgelist){
	
	std::map<int, std::set<int>> G;
	
	std::vector<std::string> tokens;
	unsigned int tok1 = 0, tok2 = 0;
	for (std::string s : edgelist){
		boost::split(tokens, s, [](char c){return c == ',';});
		tok1 = std::stoi(tokens[0]);
		tok2 = std::stoi(tokens[1]);
		
		G[tok1].insert(tok2);
		G[tok2].insert(tok1);
		tokens.clear();
	}
	std::cout << "Graph loaded!\n";
	return G;
}

int main(int argc, char* argv[])
{

	std::string data_path = argv[1]; //argv[1] contains the path of the graph.
	
	std::string s(std::istreambuf_iterator<char>(std::ifstream(data_path)
												 >> std::skipws),
				  std::istreambuf_iterator<char>());
	
	
	std::vector<std::string> tokens;
	boost::split(tokens, s, [](char c){return c == '\n';});
	
	std::map<int, std::set<int>> fast_G = read_edgelist(tokens);
	
	std::unordered_map<std::string, std::string> cfg = read_config_file(argv[2]);
	
	unsigned int start = std::stoi(cfg["start"]); //starting vertex, we'll use this for create the first graphlet
	unsigned int k = std::stoi(cfg["k"]); //the number of verteces of our graphlet in the current sampling procedure
	unsigned int max_iter = std::stoi(cfg["max_iter"]); //max number of sampling step
	unsigned int lock = std::stoi(cfg["lock"]); //how many steps before sampling
	unsigned long seed = std::stoi(cfg["seed"]);
	std::cout << "aa" << std::endl;
	srand(seed);
	
	Estimator e;
	e.sampler(fast_G, start, k, max_iter, lock);

	return 0;
}




