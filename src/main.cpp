//
//  main.cpp
//  src
//
//  Created by Lorenzo Giusti on 05/04/2019.
//  Copyright © 2019 Lorenzo Giusti. All rights reserved.
//


// TODO: random edge placement
// TODO: read edgelist
#include "Graphlet.cpp"

#include <unordered_map>
#include <unordered_set>


int main(int argc, char* argv[])
{
	
    Graph g = *new Graph("edgelist.csv");
    g.printGraph();
	std::cout << "Test Bipartition: " << (g.isBipartite(1) ? "Positive\n" : "Negative\n");
	std::cout << "Test Connection: " << (g.isConnected(1) ? "Positive\n" : "Negative\n");
	std::cout << "\n";
	
	
	std::unordered_map<int, std::unordered_set<int>> g1 ( {{1, {3,4}}, {3, {4,1}}, {4, {1,3}}} );
	std::unordered_map<int, std::unordered_set<int>> g2 ( {{1, {2,3}}, {2, {3,1}}, {3, {1,2}}} );
	std::unordered_map<int, std::unordered_set<int>> g3 ( {{3, {2,4}}, {2, {3}}, {4, {3}}} );
	std::unordered_map<int, std::unordered_set<int>> g4 ( { {1, {2,4}}, {2, {1}}, {4, {1}}} );
    Graphlet ga(g1);
	Graphlet gb(g2);
	Graphlet gc(g3);
	Graphlet gd(g4);
	
	std::unordered_map<Graphlet, std::unordered_set<Graphlet>> mg
	(
		{ 	{ga, {gb,gc, gd}},
			{gb, {ga,gc,gd}},
			{gc, {ga,gb, gd}},
			{gd, {ga, gb, gc}}
		}
	);
}




