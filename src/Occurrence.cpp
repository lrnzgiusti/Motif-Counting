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
    
    verts.reserve(graph->get_size());
    for(auto kv : graph->get_repr()) {
        verts.push_back(kv.first);
    }
    std::sort(verts.begin(), verts.end());
    for(unsigned int i=1; i<size; i++)
    {
        for(unsigned int j=0; j<i; j++)
        {
            if (graph->has_edge(verts[i], verts[j])){
                    std::cout << "Test succeed with (" << verts[i] << "," << verts[j] << ")\n";
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



OccurrenceCanonicizer::OccurrenceCanonicizer(unsigned int size) : size(size), words_needed(static_cast<size_t>(SETWORDSNEEDED(static_cast<int>(size))))
{
    g = new nauty_graph[size*words_needed];
    cang = new nauty_graph[size*words_needed];
    lab = new int[size];
    ptn = new int[size];
    orbits = new int[size];
    
    options.getcanon = MOTIVO_NAUTY_TRUE;
}

OccurrenceCanonicizer::~OccurrenceCanonicizer()
{
    delete[] g;
    delete[] cang;
    delete[] lab;
    delete[] ptn;
    delete[] orbits;
    
    nauty_freedyn();
    nautil_freedyn();
    naugraph_freedyn();
}

void OccurrenceCanonicizer::canonicize(Occurrence *occ)
{
    assert(size==occ->size);
#ifndef NEBUG
    nauty_check(MOTIVO_NAUTY_WORDSIZE, static_cast<int>(words_needed), static_cast<int>(size), NAUTYVERSIONID);
#endif
    
    EMPTYGRAPH(g, words_needed, size);
    
    for(unsigned int i=1; i<size; i++)
    {
        for (unsigned int j = 0; j < i; j++)
        {
            if (occ->has_edge(i, j))
                ADDONEEDGE (g, i, j, words_needed);
        }
    }
    
    densenauty(g, lab, ptn, orbits, &options, &stats, static_cast<int>(words_needed), static_cast<int>(size), cang);
    
    //From the nauty manual: the value of lab on return is the canonical labelling
    //of the graph. Precisely, it lists the vertices of g in the order in which they need to
    //be relabelled to give canong
    
    std::vector<int> new_verts(size);
    memcpy(&new_verts, &occ->verts, sizeof(int)*size);
    
    for(unsigned int i=0; i<size; i++)
        occ->verts[i] = new_verts[ lab[i] ];
    
    memset(occ->edges, 0, sizeof(uint8_t)*Occurrence::binary_footprint_bytes);
    for(unsigned int i=1; i<size; i++)
    {
        nauty_set* row = GRAPHROW(cang, i, words_needed);
        for(unsigned int j = 0; j < i; j++)
        {
            if( ISELEMENT( row, j) )
                occ->add_edge(i, j);
        }
    }
    
    occ->text_footprint_buffer[0]=0; //Invalidate text footprint
}


