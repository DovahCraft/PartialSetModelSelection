//#pragma once
#include <iostream>
#include <map>

enum class ModelCapMessages {NO_MODEL_CAP, STD_MODEL_CAP=3};



struct Model {
    Model(int model_size, double loss) : model_size(model_size), loss(loss) {}
    //Number of segments (k-value)
    int model_size = 0;
    //Loss associated with the given model 
    double loss = 0.0;
};

using breakPoint = std::pair<double, Model>;



class ModelSelectionMap {
public:
    ModelSelectionMap(ModelCapMessages cap);

    void insert(double penalty, Model currentModel);

    ModelSelectionMap();
    


private:
    ModelCapMessages maxModels = ModelCapMessages::NO_MODEL_CAP;
    std::map<double, Model> breakpoints;
};
//Class to embody Model,Boolean pairs for model selection.
struct MinimizeResult {
    int model_size = 0;
    bool certain = false;
};




//Function definitions
void insert(double penalty, Model currentModel, ModelSelectionMap currentMap);
std::pair<int, bool> minimize(double penalty);
std::pair<int, int> solver(double penalty);