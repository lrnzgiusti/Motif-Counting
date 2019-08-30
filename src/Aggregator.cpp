//
//  Aggregator.cpp
//  dummy
//
//  Created by Lorenzo Giusti on 31/07/2019.
//  Copyright © 2019 Lorenzo Giusti. All rights reserved.
//

#include "Aggregator.hpp"


std::string exec(const char* cmd){
    /*
     This function executes a standard linux command and the the output into a string variable
     */
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

std::map<int, std::map<int, float>>  load_sample(std::string filename, int n_of_samples){
    /**
     This function loads a single sample: a sample is physically a file containing the frequencies for each step.
     */
    std::map<int, std::map<int, float>>  sample;
    std::map<int, float> motif_to_freq;
    
    std::string s(std::istreambuf_iterator<char>(std::ifstream(filename)
                                                 >> std::skipws),
                  std::istreambuf_iterator<char>());
    
    
    std::vector<std::string> lines;
    boost::split(lines, s, [](char c){return c == '\n';});
    
    lines.erase(lines.begin()); //remove header
    lines.erase(lines.end()-1); //remove final newline
    
    std::vector<std::string> tokens;
    for (std::string s : lines){
        boost::split(tokens, s, [](char c){return c == ' ';});
        tokens.erase(tokens.end()-1); //remove the ending \n
        for(int i = 1; i < tokens.size()/2; i++){
            motif_to_freq[k5_string_to_minhash[tokens[2*i-1]]] = std::stof(tokens[2*i])/n_of_samples; 
            try {
                sample[std::stoi(tokens[0])] = motif_to_freq;
            } catch (...) {
                sample[std::stoi(tokens[0])] = motif_to_freq;
            }
            
            
        }
        tokens.clear();
    }
    return sample;
}


std::map<int, std::map<int, float>> load_samples(){
    std::map<int, std::map<int, float>> time_to_avg;
    
    std::string ls_result = exec("ls /Users/ince/Desktop/tmp/*.txt");
    std::vector<std::string> files;
    boost::split(files, ls_result, [](char c){return c == '\n';});
    files.erase(files.end()-1); //remove final newline
    const unsigned int n_of_samples = (unsigned int)files.size();
    for(std::string file : files){
        std::string file_as_string(std::istreambuf_iterator<char>(std::ifstream(file)
                                                     >> std::skipws),
                      std::istreambuf_iterator<char>());
        
        
        std::vector<std::string> lines;
        boost::split(lines, file_as_string, [](char c){return c == '\n';});
        
        lines.erase(lines.begin()); //remove header
        lines.erase(lines.end()-1); //remove final newline
        
        std::vector<std::string> tokens;
        for (std::string line : lines){
            boost::split(tokens, line, [](char c){return c == ' ';});
            tokens.erase(tokens.end()-1); //remove the ending \n
            
            
            for(int i = 1; i <= tokens.size()/2; i++){
                time_to_avg[std::stoi(tokens[0])][k5_string_to_minhash[tokens[2*i-1]]] += std::stof(tokens[2*i])/n_of_samples;
            }
            tokens.clear();
        }
    }
    return time_to_avg;
}



void perform_L1(std::map<int, float> stationary_distro){
    std::ofstream of;
    of.open("l1.csv");
    if(!of) std::cerr << "Output file not created.\n";
    of << "x,y\n";
    std::map<int, std::map<int, float>> avg_sample = load_samples();
    float current_l1 = 0.0;
    for(const std::pair<int, std::map<int, float>> &iteration : avg_sample){
        for(const std::pair<int, float> motif_to_freq : iteration.second){
            current_l1 += std::abs(motif_to_freq.second - stationary_distro[motif_to_freq.first]);
        }
        of << iteration.first << "," << current_l1 << "\n";
        current_l1 = 0;
    }
}
