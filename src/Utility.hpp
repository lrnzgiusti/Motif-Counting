//
//  Utility.hpp
//  dummy
//
//  Created by Lorenzo Giusti on 18/04/2019.
//  Copyright © 2019 Lorenzo Giusti. All rights reserved.
//

#ifndef Utility_hpp
#define Utility_hpp


#include <vector>
#include <unordered_map>


template<class T> float l1_diff(std::unordered_map<T, float> v1, std::unordered_map<T, float> v2, int t){
    if(v1.size() != v2.size()){
        std::cerr << "Distribution size is different\n";
        return 0.0;
    }
    double sum = 0;
    
    for(std::pair<T, float> p : v1){
        v1.at(p.first) /= t;
        v2.at(p.first) /= t;
    }
    
    for(std::pair<T, float> p : v1){
        sum += abs(v1.at(p.first) - v2.at(p.first));
    }

    return sum;
}


float l1_diff(std::vector<float> v1, std::vector<float> v2, int t){
    if(v1.size() != v2.size()){
        std::cerr << "Distribution size is different\n";
        return 0.0;
    }
    double sum = 0;
    for(int i = 0; i < v1.size(); i++){
        v1[i] /= t;
        v2[i] /= t;
    }
    
    for(int i = 0; i < v1.size(); i++){
        sum += abs(v1[i] - v2[i]);
    }
    std::cout << "\n" << sum ;
    std::cout << "\n";
    return sum;
}


#endif /* Utility_hpp */
