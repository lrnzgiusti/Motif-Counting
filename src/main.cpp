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
#include <sstream>
#include <string>
#include "Utility.hpp"
#include "Occurrence.cpp"


int main(int argc, char* argv[])
{
	
	srand(time(NULL));

	
	std::stringstream ss;
	ss <<  "/Users/ince/Desktop/bressan_data/" << argv[1];
	
	std::string filename = ss.str();
	
	Graph G(filename);
	Estimator e;
	assert(not G.isBipartite());
	//std::unordered_map<Graphlet, float> Gk_distro = e.sampler(G, 1, 3);//e.random_walk(G, 1, 4);
	//save_distribution(Gk_distro, "/Users/ince/Desktop/distro_3.dat");
	
	std::unordered_map<Graphlet, float> Gk_distro = e.sampler(G, 1, 3);
	
	std::unordered_map<std::string, double> motif_distro;
	
	for(std::pair<Graphlet, float> p : Gk_distro){
		Occurrence *o = new Occurrence(p.first.get_size(), &p.first);
		OccurrenceCanonicizer oc(p.first.get_size());
		oc.canonicize(o);
		std::cout << o->text_footprint() << "\t" << p.first << "\t" <<  p.second << "\n";
		motif_distro[o->text_footprint()] += p.second;
	}
	
	std::cout << std::endl;
	for (auto kv : motif_distro){
		std::cout << kv.first << "\t" << kv.second << "\n";
	}
	
}




