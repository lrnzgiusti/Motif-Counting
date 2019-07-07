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
	
	
	
	
	srand(time(NULL));
	std::stringstream ss;
	//ss <<  "/Users/ince/Desktop/bressan_data/" << argv[1];
	ss << "/Users/ince/Desktop/bressan_data/facebook.ascii";
	std::string filename = ss.str();
	Graph G(filename);
	Estimator e;
	//assert(not G.isBipartite());
	
	//std::unordered_map<std::string, float> motif_distro = e.sampler_test(G, 1, std::stoi(argv[2]), std::stoi(argv[3]));
	std::unordered_map<std::string, float> motif_distro = e.sampler_test(G, 1, 3, 200000);

	std::cout << std::endl;
	for (auto kv : motif_distro){
		std::cout << kv.first << "\t" << kv.second << "\n";
	}
	
}




