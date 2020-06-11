#ifndef MODELSELECTIONUTILS_H
#define MODELSELECTIONUTILS_H

#include <iostream>
#include <map>
#include <vector>

#include "ModelSelectionUtils.hpp"

//Struct to embody Model,Boolean pairs for model selection path records.
struct MinimizeResult {
    MinimizeResult();
    MinimizeResult(std::pair<double,double> inputModelRange);
    bool isValidRange(std::pair<double,double> inputModelRange);
    bool certain = false;
    //Stores the potential models that could encompass a penalty query. Identical first and second value if certain (solved).
    std::pair<double, double> optimalModels;
    
};

#endif