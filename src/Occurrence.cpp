//
//  Occurrence.cpp
//  dummy
//
//  Created by Lorenzo Giusti on 16/06/2019.
//  Copyright © 2019 Lorenzo Giusti. All rights reserved.
//

#include "Occurrence.hpp"


constexpr unsigned int Occurrence::binary_footprint_bits;
constexpr unsigned int Occurrence::binary_footprint_bytes;
constexpr unsigned int Occurrence::text_footprint_bytes;

Occurrence::Occurrence(const unsigned int size, const Graph *graph) : size(size)
{
    std::vector<int> keys;
    keys.reserve(graph->get_size());
    for(auto kv : graph->get_repr()) {
        keys.push_back(kv.first);
    }
    std::sort(keys.begin(), keys.end());
    for(unsigned int i=1; i<size; i++)
    {
        for(unsigned int j=0; j<i; j++)
        {
            if (graph->has_edge(keys[i], keys[j])){
                    std::cout << "Test succeed with (" << keys[i] << "," << keys[j] << ")\n";
                    std::cout << "Adding (" << i << "," << j << ")\n";
                    add_edge(i, j);
            }
        }
    }
}

Occurrence::Occurrence(const unsigned int size, const uint8_t* edges) {
    this->size = size;
    memcpy(this->edges, edges, binary_footprint_bytes);
}



const char* Occurrence::text_footprint() const
{
    if(text_footprint_buffer[0]==0)
    {
        for(unsigned int i=0; i<binary_footprint_bytes; i++)
        {
            text_footprint_buffer[2*i]= static_cast<char>('A'+ (edges[i]>>4u));
            text_footprint_buffer[2*i+1]= static_cast<char>('A'+ (edges[i] & 0x0Fu));
        }
    }
    
    return text_footprint_buffer;
}
