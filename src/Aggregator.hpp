//
//  Aggregator.hpp
//  dummy
//
//  Created by Lorenzo Giusti on 31/07/2019.
//  Copyright © 2019 Lorenzo Giusti. All rights reserved.
//

#ifndef Aggregator_hpp
#define Aggregator_hpp

#include <stdio.h>
#include <map>
#include <string>
#include <vector>

std::map<int, float> k5_dblp_2011 = {{75, 0.32172554864928504},
    {77, 0.4752733242162422},
    {86, 0.11112141416249405},
    {79, 0.0373751373070648},
    {117, 0.01930238004813544},
    {87, 0.02396232711173203},
    {94, 0.0007458927268653942},
    {236, 2.6222662578482457e-05},
    {235, 0.001423172835951017},
    {119, 0.0013732879293924466},
    {95, 0.0023610416806287607},
    {237, 4.8492934379157085e-05},
    {222, 1.373202252061797e-06},
    {223, 8.697430871167306e-05},
    {127, 0.0039929732350526045},
    {239, 0.0001352680900710047},
    {254, 3.0559697159907583e-06},
    {507, 1.2831783543364822e-06},
    {255, 0.00034599533513098957},
    {511, 6.0716460063589146e-05},
    {1023, 0.0006341179558989391}};


std::map<int, float> k5_wordassociation_2011 = {{75, 0.2540039571534765},
    {77, 0.4491536196759397},
    {86, 0.1942149550093916},
    {79, 0.04232224592514201},
    {117, 0.020346110053308032},
    {87, 0.026555907922911376},
    {94, 0.004717838774906891},
    {236, 0.0003073065154985832},
    {235, 0.0006751985994825108},
    {119, 0.0021135620256943093},
    {95, 0.004391677725336605},
    {237, 0.000293044123825747},
    {222, 2.6085197294637935e-05},
    {223, 0.00011255802685872781},
    {127, 0.0004520620188747744},
    {239, 0.0002171249337547622},
    {254, 2.2812703635584803e-05},
    {507, 6.006099313291176e-06},
    {255, 5.975521274558e-05},
    {511, 7.266167476907601e-06},
    {1023, 9.061351318354616e-07}};

std::map<int, float> k5_cit_Patents = {{75, 0.35594737473633636},
    {77, 0.37730548695844135},
    {86, 0.16139718287673854},
    {79, 0.021277903553317316},
    {117, 0.012311359849033907},
    {87, 0.0158554130247135},
    {94, 0.02997696723230399},
    {236, 0.00028194462950698816},
    {235, 0.0002763746617746861},
    {119, 0.0026991043064872227},
    {95, 0.0039271383130026205},
    {237, 0.0007271019777751334},
    {222, 0.01581618763579486},
    {223, 0.0007015013707754693},
    {127, 0.0002874409720124054},
    {239, 0.00025810021160989485},
    {254, 0.0006696089500455035},
    {507, 0.00013138153285473556},
    {255, 0.00010884507517672232},
    {511, 4.057107409218424e-05},
    {1023, 3.011058206601896e-06}};

std::map<int, float> k5_facebook = {{75, 0.1720740379552569},
    {77, 0.4496438091680514},
    {86, 0.22015253649174962},
    {79, 0.04141912083621441},
    {117, 0.03850612543906294},
    {87, 0.04571026607593683},
    {94, 0.0096355668553939},
    {236, 0.0006956241242889949},
    {235, 0.0014584324654846922},
    {119, 0.006665191767318252},
    {95, 0.008679388302186185},
    {237, 0.001159471893096328},
    {222, 0.00010480133857943658},
    {223, 0.00027542965130180225},
    {127, 0.002079004409654997},
    {239, 0.0009686953580805336},
    {254, 0.00015287172409129258},
    {507, 5.7563362167680934e-05},
    {255, 0.00043022256565330394},
    {511, 0.0001067338966920096},
    {1023, 2.5106319738476838e-05}};




std::map<int, float> k3_facebook = {{3 , 0.9453808},
    {7 , 0.0546192}};


std::map<int, float> k3_test = {{3 , 0.4285714},
    {7 , 0.5714286}};

std::map<std::string, int> k5_string_to_minhash ={  {"IFMAAA", 117}, {"MIMAAA", 236}, {"CPMAAA", 127},
    {"BPIAAA", 222}, {"BPMAAA", 223}, {"ADMAAA", 75},
    {"APIAAA", 94}, {"EPMAAA", 239}, {"DPMAAA", 255},
    {"HPMAAA", 511}, {"IHMAAA", 235}, {"AHMAAA", 79},
    {"BFIAAA", 86}, {"BFMAAA", 87}, {"MPMAAA", 507},
    {"JFMAAA", 237}, {"AHEAAA", 77}, {"COMAAA", 119},
    {"PPMAAA", 1023}, {"MNMAAA", 254}, {"APMAAA", 95} };

std::map<std::string, int> k3_string_to_minhash = {{"OAAAAA", 7}, {"GAAAAA", 3},};

//load a single sample file into <iteration, <motif,frequency>> map
std::map<int, std::map<int, float>>  load_sample(std::string filename, int n);

//load all the samples inside a directory, This converts the txt files in output from the random walk into a list of dictionary containing the motifs
std::map<int, std::map<int, float>> load_samples();



void perform_L1(std::map<int, float> stationary_distro);
#endif /* Aggregator_hpp */
