//
//  main.cpp
//  src
//
//  Created by Lorenzo Giusti on 05/04/2019.
//  Copyright © 2019 Lorenzo Giusti. All rights reserved.
//

#include "Graph.cpp"
#include <unordered_set>
#define MAX_NUM_NODES 10
int main(int argc, char* argv[])
{
    
    srand((int)time(NULL));
    Graph g; //G dev'essere undirected, fare un check
    g.printGraph();
    
}
