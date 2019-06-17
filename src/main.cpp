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
#include "Occurrence.cpp"


int main(int argc, char* argv[])
{
	
	srand(time(NULL));
	
	
	Graph G("/Users/ince/Desktop/bressan_data/edgelist2.csv");
	Estimator e;
	assert(not G.isBipartite());
	//std::unordered_map<Graphlet, float> Gk_distro = e.sampler(G, 1, 3);//e.random_walk(G, 1, 4);
	//save_distribution(Gk_distro, "/Users/ince/Desktop/distro_3.dat");
	std::unordered_map<Graphlet, float> Gk_distro = e.sampler(G, 1, 3);
	Occurrence::OccurrenceFootprintHash ofh;
	
	
	for(std::pair<Graphlet, float> p : Gk_distro){
		Occurrence o(p.first.get_size(), &p.first);
		std::cout<< ofh(o) << "\t";
		std::cout << o.text_footprint() << "\t" << p.first << "\t" <<  p.second << "\n";
	}
}




