#pragma once
#include <iostream>
#include <map>
#include <vector>


enum class ModelMapMessages {NO_MODEL_CAP, STD_MODEL_CAP=3};


struct Model {
    Model(int model_size, double loss) : model_size(model_size), loss(loss) {}
    //Number of segments (k-value)
    int model_size = 0;
    //Loss associated with the given model 
    double loss = 0.0;

    //Range of penalties for which this model is optimal.
    std::pair<double,double> optimalPenaltyRange;
};


//Class to embody Model,Boolean pairs for model selection path records.
struct MinimizeResult {
    MinimizeResult();
    MinimizeResult(std::pair<double,double> inputModelRange);
    bool isValidRange(std::pair<double,double> inputModelRange);
    bool certain = false;
    //Stores the potential models that could encompass a penalty query. Identical first and second value if certain (solved).
    std::pair<double, double> optimalModels;
    
};
//struct TestedPair may be better here for more readability
using TestedPair = std::pair<double, Model>;





class ModelSelectionMap {
public:
    //Map constants and return codes.
    const double EMPTY_MAP_QUERY = 0;
    const double EMPTY_MAP_ERR = -99999;
    const double DEFAULT_PENALTY = -9999;

    //Method headers
    ModelSelectionMap();

    ModelSelectionMap(ModelMapMessages cap);


    /*
     Function name(s): insert
     Algorithm: Inserts a new model into our partial set (map) data structure with a penalty modeling FPOP
     Precondition: The model is formatted correctly and the
        penalty is a valid double.
     Postcondition: Inserts the model into the data structure.
        No return type, maybe return success code eventually.
     Exceptions: correctly and appropriately (without program failure)
        responds to and reports failure to insert the model.
     Note: none
    */
    void insert(double penalty, Model currentModel);

    /*
    Function name: insert (overloaded)
    Algorithm: Inserts a new model into our partial set (map) without a penalty, modeling
    binary segmentation and other constrained style solvers. 
    Precondition: The model is formatted correctly and the
    penalty is a valid double.
    Postcondition: Inserts the model into the data structure.
    No return type, maybe return success code eventually.
    Exceptions: correctly and appropriately (without program failure)
        responds to and reports failure to insert the model.
    Note: none
    */
    void insert(Model currentModel);


    double getNewPenaltyList();

    int getModelCount();

    void displayMap();


    /*
    Function name: Solver
    Algorithm:

    Precondition: for correct operation, the passed penalty is a valid
    float value.
    Postcondition:
    Exceptions: none yet. 
    Note: none
    */
    std::pair<int, int> solver(double penalty);


    /*
    Function name: Minimize
    Algorithm: Acquires a penalty value lambda and returns a minimization result consisting of:
    (the k value corresponding to the selected model, a boolean showing its accuracy)
    Boolean values consist of: F for unsure, and T for a sure pairing.
    Precondition: for correct operation, the passed penalty is a valid
    float value.
    Postcondition: in correct operation, computes what model is optimal
    for the passed penalty, and gives a boolean signifying its accuracy.
    Both these values are passed back as a struct.
    Exceptions: none?
    Note: none
    */
    MinimizeResult minimize(double penalty);

    
    //Wrapper struct to contain a list of MinimizeResults
    std::vector<MinimizeResult> minResultVec; 

    ModelMapMessages maxModels = ModelMapMessages::NO_MODEL_CAP;
    std::map<double, Model> testedPairs;
    int modelCount = 0;
};





