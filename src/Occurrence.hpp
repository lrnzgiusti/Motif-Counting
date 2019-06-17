//
//  Occurrence.hpp
//  dummy
//
//  Created by Lorenzo Giusti on 16/06/2019.
//  Copyright © 2019 Lorenzo Giusti. All rights reserved.
//

#ifndef Occurrence_hpp
#define Occurrence_hpp
#include <climits>
#include "Graph.hpp"
class Occurrence
{
    
    
public:
    //i,j in {0,...,15}
    //edge (i,j) with i>j is in position sum_{k=1}^(i-1) k + j = (i-1)*i/2 + j in edges
    //last bit is the one corresponding to i=15, j=14 => at most 119 bits are need (14 bytes, 7 bits)
    constexpr static unsigned int binary_footprint_bits = 19;
    constexpr static unsigned int binary_footprint_bytes = (binary_footprint_bits + CHAR_BIT - 1) / CHAR_BIT; //round up (to 15 bytes)
    constexpr static unsigned int text_footprint_bytes = binary_footprint_bytes * 2;
    
    struct OccurrenceFootprintHash
    {
        inline size_t operator()[[gnu::hot,gnu::flatten]] (const Occurrence *key) const
        {
            size_t seed;
            seed = key->is_valid()?0xcb7fedb03a45866f:0xb896186490f1c8e9;
            
            const char* p = key->binary_footprint();
            for (unsigned int i = 0; i < Occurrence::binary_footprint_bytes; i++)
                seed ^= static_cast<unsigned char>(p[i])*0xff51afd7ed558ccd +0x9e3779b9 + (seed << 6) + (seed >> 2);
            
            return seed;
        }
        inline size_t operator()[[gnu::hot,gnu::flatten]] (const Occurrence &key) const
        {
            size_t seed;
            seed = key.is_valid()?0xcb7fedb03a45866f:0xb896186490f1c8e9;
            
            const char* p = key.binary_footprint();
            for (unsigned int i = 0; i < Occurrence::binary_footprint_bytes; i++)
                seed ^= static_cast<unsigned char>(p[i])*0xff51afd7ed558ccd +0x9e3779b9 + (seed << 6) + (seed >> 2);
            
            return seed;
        }
    };
    
    struct OccurrenceFootprintEquality
    {
        inline bool operator() [[gnu::hot,gnu::flatten]] (const Occurrence *occ1, const Occurrence *occ2) const
        {
            return (occ1->is_valid()==occ2->is_valid()) && !memcmp(occ1->binary_footprint(), occ2->binary_footprint(), Occurrence::binary_footprint_bytes);
        }
        inline bool operator() [[gnu::hot,gnu::flatten]] (const Occurrence &occ1, const Occurrence &occ2) const
        {
            return (occ1.is_valid()==occ2.is_valid()) && !memcmp(occ1.binary_footprint(), occ2.binary_footprint(), Occurrence::binary_footprint_bytes);
        }
    };
    
    struct OccurrenceFootprintLess
    {
        inline bool operator()[[gnu::hot,gnu::flatten]] (const Occurrence *occ1, const Occurrence *occ2) const
        {
            return (occ1->is_valid()==occ2->is_valid()) && (memcmp(occ1->binary_footprint(), occ2->binary_footprint(), Occurrence::binary_footprint_bytes) < 0);
        }
        inline bool operator()[[gnu::hot,gnu::flatten]] (const Occurrence &occ1, const Occurrence &occ2) const
        {
            return (occ1.is_valid()==occ2.is_valid()) && (memcmp(occ1.binary_footprint(), occ2.binary_footprint(), Occurrence::binary_footprint_bytes) < 0);
        }
    };
    
private:
    unsigned int size;
    uint8_t edges[binary_footprint_bytes] = { 0 };
    mutable uint64_t spanning_trees = 0;
    mutable char text_footprint_buffer[text_footprint_bytes + 1] = { 0 }; //Add null-terminator
    
    inline void add_edge(unsigned int i, unsigned int j)
    {
        unsigned int pos = (i - 1) * i / 2 + j;
        edges[pos / 8] |= static_cast<uint8_t>(0b10000000 >> (pos % 8));
    }
    
    
public:
    constexpr Occurrence() : size(0)
    {} //Empty constructor to take advantage of Stack allocation
    
    Occurrence(const unsigned int size, const Graph* graph);
    Occurrence(const unsigned int size, const uint8_t* edges);
    
    inline bool has_edge(unsigned int i, unsigned int j) const //FIXME: Could be invoked with j>=i
    {
        unsigned int pos = (i - 1) * i / 2 + j;
        return (edges[pos / 8] & (0b10000000u >> (pos % 8))) != 0;
    }
    

    
    const char* binary_footprint() const
    {
        return reinterpret_cast<const char*>(edges);
    }
    
    //Lazily computes the text footprint. Const is fine because the footprint is mutable
    const char* text_footprint() const;
    
    bool is_valid() const
    {
        return size != 0;
    }
    
    unsigned int get_size() const
    {
        return size;
    }
    
};


#endif /* Occurrence_hpp */
