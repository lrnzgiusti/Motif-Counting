//
//  Utility.cpp
//  dummy
//
//  Created by Lorenzo Giusti on 18/04/2019.
//  Copyright © 2019 Lorenzo Giusti. All rights reserved.
//

#include "Utility.hpp"


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
    
    return sum;
}
