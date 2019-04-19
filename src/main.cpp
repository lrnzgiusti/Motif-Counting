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


int main(int argc, char* argv[])
{
	/*
    Graph g = *new Graph("/Users/ince/Desktop/edgelist.csv");
    g.printGraph();
	std::cout << "Test Bipartition: " << (g.isBipartite(1) ? "Positive\n" : "Negative\n");
	std::cout << "Test Connection: " << (g.isConnected(1) ? "Positive\n" : "Negative\n");
	std::cout << "\n" <<"\n";
	*/
	
	
	srand(time(NULL));
	Graph random_graph(1,2,8);
	random_graph.printGraph();
	std::cout << "Test Bipartition: " << (random_graph.isBipartite(1) ? "Positive\n" : "Negative\n");
	std::cout << "Test Connection: " << (random_graph.isConnected(1) ? "Positive\n" : "Negative\n");
	std::cout << "\n" <<"\n";
	
	
	
	std::vector<float> vec = random_graph.random_walk();
	for(int i = 1; i < vec.size(); i++) std::cout<<"index: " << i << " Value: "<< vec[i] << "\n";
	std::cout << "\n";
	std::cout << "Number of edges: " << random_graph.get_num_of_edges() << "\n";
	/*
	int k,s;
	std::cin >> s;
	std::cin >> k;
	Estimator().pick_the_first(random_graph, s, k).printGraph();
	*/
	/*
	
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
	
	for(std::pair<Graphlet, std::unordered_set<Graphlet>> x : mg){
		x.first.printGraph(); 
		std::cout << "\n";
		for(Graphlet g : x.second){
			g.printGraph();
			std::cout << "--------------------\n";
		}
		std::cout << "\n\n";
	}
	 */
	
	
}




