#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <list>
#include <set>
#include <string>
#include <math.h>
//Global Constants
const int MODELSIZE_CONSTANT = -9998;
const double PENALTY_CONSTANT = -9999;


struct Model {
    //Number of segments (k-value)
    int modelSize = 0;
    //Loss associated with the given model 
    double loss = 0.0;
    //Penalty associated with this model
    double penalty;

    //Create a model if it has valid modelSize and loss values given. 
    Model(int inputSize, double inputLoss) {
        if(inputSize >= 0 && inputLoss >= 0){
            modelSize = inputSize;
            loss = inputLoss;
        }
        //TODO: The enum will break things if the user tries to create a model with -9998 modelsize. It will not be invalidated! 
        else if(inputSize == MODELSIZE_CONSTANT)
            modelSize = MODELSIZE_CONSTANT;
    
        else{
            throw std::out_of_range("[ ERROR ] Cannot create model with negative size or loss values! Provided model_size = " 
            + std::to_string(inputSize) + ", loss = " + std::to_string(inputLoss) + "\n");
        }
    }

    //Custom operators overloaded to work with insertion and minimize queries and lower_bound.     
};

//Operators are actually meant to be specified outside of the 
//nline bool operator==(const Model& lhs, const Model& rhs){return lhs.modelSize == rhs.modelSize || lhs.penalty == rhs.penalty;}
bool operator<(const Model& lhs, const Model& rhs){
    if(rhs.penalty == PENALTY_CONSTANT){
        //Additional testing messages if needed.
        //std::cout << "MODELSIZE COMPARISON  \n";
        //std::cout << "LHS MODELSIZE: " << lhs.modelSize << "  RHS MODELSIZE: " << rhs.modelSize <<"\n";
        return rhs.modelSize < lhs.modelSize;
    }    
    else{
        return lhs.penalty < rhs.penalty;
    }
}
//inline bool operator>(const Model& lhs, const Model& rhs){return lhs.modelSize > rhs.modelSize || lhs.penalty < rhs.penalty;}

class ModelSelectionMap{
public:
    std::set<Model> path;

    //Basic insertion method
    void insert(Model toInsert);

    //Two lower_bound methods
    std::set<Model>::iterator lowerBoundModelSize(int modelSizeQuery);
    std::set<Model>::iterator lowerBoundPenalty(double penaltyQuery); 

    //Display
    void displayPathSet();
};

//Demos
void runLowerBoundsDemo();
void runDualKeyDemo();
void runTwoMapDemo();

//Breakpoint computation
double findBreakpoint(Model firstModel, Model secondModel);

//Display method
void displayDualMap(std::map<std::pair<double, int>, Model> modelMap);
void displayModelSizeMap(std::map<int, Model> modelMap);
void displayPenaltyMap(std::map<double, Model> modelMap);