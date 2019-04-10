//
//  main.cpp
//  src
//
//  Created by Lorenzo Giusti on 05/04/2019.
//  Copyright © 2019 Lorenzo Giusti. All rights reserved.
//


// TODO: random edge placement
// TODO: read edgelist
#include "Graph.cpp"
#include "Graphlet.cpp"

#include <unordered_map>
#include <unordered_set>


int main(int argc, char* argv[])
{
    srand((int)time(NULL));
    Graph g;
    g.printGraph();
    std::cout << "Test Bipartition: " << (g.isBipartite() ? "Positive\n" : "Negative\n");
    
    std::unordered_map<int, std::unordered_set<int>> g1 ( {{1, {2,3}}, {2, {3}}, {3, {}}} );
    std::unordered_map<int, std::unordered_set<int>> g2 ( {{1, {2,3,4}}, {2, {3}}, {3, {}}} );
    Graphlet gg(g1);
    Graphlet gz(g2);
    std::unordered_map<Graphlet, std::unordered_set<Graphlet>> mapp ({{gg, {gz}}, {gz, {gg}}});
    //TODO: test if the behaviour of this map is good for our purposes.
    //TODO: introduce a class that provides us utility fuction <isBipartite, isConnected>(const Graph& g)const;
}



