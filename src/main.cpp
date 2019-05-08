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
	
	srand(time(NULL));
	
	
	Graph G("/Users/ince/Desktop/edgelist.csv");
	//Graph G(2,5,7);
	G.printGraph();
	std::cout << "\n";
	Estimator e;
	
	//std::unordered_map<Graphlet, float> Gk_distro = e.sampler(G, 1, 3);//e.random_walk(G, 1, 4);
	//save_distribution(Gk_distro, "/Users/ince/Desktop/distro_3.dat");
	std::unordered_map<Graphlet, float> Gk_distro = e.random_walk(G, 1, 3);
	
	
	for(std::pair<Graphlet, float> p : Gk_distro){
		std::cout << p.first << "," <<  p.second << "\n";
	}
}




