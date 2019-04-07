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

#include <unordered_map>
#include <unordered_set>

int main(int argc, char* argv[])
{
    srand((int)time(NULL));
    Graph g;
    g.printGraph();
    std::cout << "Test Bipartition: " << (g.isBipartite() ? "Positive\n" : "Negative\n");
}



