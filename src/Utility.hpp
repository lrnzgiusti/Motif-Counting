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

template<class T> float l1_diff(std::unordered_map<T, float> &v1, std::unordered_map<T, float> &v2, int t){
    if(v1.size() != v2.size()){
        return -1; //Distribution size are different
    }
    double sum = 0;
    
    for(std::pair<T, float> p : v1){
        sum += abs((v1.at(p.first) - v2.at(p.first))/t);
    }
    std::cout << sum << "\n";
    return sum;
}


std::unordered_map<std::string, std::string> read_config_file(std::string path){
    std::unordered_map<std::string, std::string> cfg;
    
    std::ifstream fptr;
    fptr.open(path);
    
    if(!fptr){
        std::cerr << "Unable to open cfg file!\tTry Again!\n";
        return cfg;
    }
    
    std::string config_row;
    std::vector<std::string> tokens;
    std::string key, value;
    while(std::getline(fptr, config_row)){
        boost::split(tokens, config_row, [](char c){return c == ':';});
        key = tokens[0];
        value = tokens[1];
        cfg[key] = value;
        tokens.clear();
    }
    fptr.close();
    
    return cfg;
}

float l1_disto_diff(std::unordered_map<int, float> real, std::unordered_map<int, float> theo, int t){
    if(theo.size() != real.size()){
        return 1e6; //Distribution size are different, let's get more samples
    }
    double sum = 0;

    for(std::pair<int, float> p : real){
        real.at(p.first) /= t;
    }
    
    for(std::pair<int, float> p : real){
        sum += abs(real.at(p.first) - theo.at(p.first));
    }
    return sum;
}




template<class T> void normalize_distribution(std::unordered_map<T, float> &distro){
    float sum = 0;
    for(std::pair<T, float> p : distro){
        sum += p.second;
        
    }
    
    for(std::pair<T, float> p : distro){
        distro.at(p.first) /= sum;
    }
}

template<class T> void save_distribution(std::unordered_map<T, float> distro, std::string filename){
    std::ofstream fptr;
    fptr.open(filename);
    
    if(!fptr){
        std::cerr << "Unable to open the outputfile!\tTry Again!\n";
        return;
    }
    
    for(std::pair<T, float> p : distro){
        fptr << p.first << "," <<  p.second << "\n";
    }
    
    fptr.close();
    return;
}

float l1_diff(std::vector<float> v1, std::vector<float> v2, int t){
    if(v1.size() != v2.size()){
        return -1; //Distribution size are different
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

#endif /* Utility_hpp */
