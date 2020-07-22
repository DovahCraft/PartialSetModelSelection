#pragma once
#include <iostream>
#include <map>
#include <vector>

struct Breakpoint {
    double penalty;
    std::pair<int, int> optimalModelSizes;
};

//Struct to embody Model,Boolean pairs for model selection path records.
struct MinimizeResult {
    MinimizeResult(int modelSize = 1, bool certain = false);
    bool certain;
    int modelSize;
};

struct Model {
    Model(int inputSize, double inputLoss) : modelSize(inputSize), loss(loss), isPlaceHolder(false) {
        if(inputSize >= 0 && inputLoss >= 0){
            modelSize = inputSize;
            loss = inputLoss;
        }
        else{
            throw std::out_of_range("[ ERROR ] Cannot create model with negative size or loss values! Provided model_size = " 
            + std::to_string(inputSize) + ", loss = " + std::to_string(inputLoss) + "\n");
        }
    }
    //Number of segments (k-value)
    int modelSize = 0;
    //Loss associated with the given model 
    double loss = 0.0;
    int modelSizeAfter; //Used for next Model (the after flag in psuedocode)
    MinimizeResult minimizeResult;
    bool isPlaceHolder; //Used to determine if the key at 0 is the initial key we insert.
    std::pair<double,double> optimalPenalties; //Used to filter out unnecessary penalties within optimal range. 
};

//struct penaltyModelPair may be better here for more readability
using PenaltyModelPair = std::pair<double, Model>;

class ModelSelectionMap {
public:
    int insertedModels; //This is used to determine if the map is 'empty' as the initial model inserted scews isEmpty() counts.
    const double modelSizeCap;

    std::map<double,Model>::iterator lastInsertedPair; //Holds the previously computed breakpoint, if it exists, for use in constant time insertion. 
    //Map struct to hold penalty and model pairings from inserts.
    std::map<double, Model> penaltyModelMap; 
    //Vector to hold new candidate penalties and breakpoints to give new information from minimize.
    std::vector<double> newPenalties;
    //Method headers
    //Default value of INFINITY for no passed cap
    ModelSelectionMap(double maxModels = INFINITY);
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
    void insert(double penalty, int modelSize, double loss);

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
    void insert(int modelSize, double loss);

     /*
    Function name: remove
    Algorithm: Removes a model/penalty pairing if deemed to no longer be optimal.  
    Precondition: The model is formatted correctly and the
    penalty is a valid double.
    Postcondition: Removes the model from the data structure.
    Returns the removed pair.
    Exceptions: correctly and appropriately (without program failure)
        responds to and reports failure to insert the model.
    Note: none
    */
    void remove(std::map<double, Model>::iterator toRemove);


    /*
    Function name: getNewpenaltyQuery
    Algorithm: O(1) or O(log N) query of a penalty value that will result in new information. 
    Precondition: for correct operation, the passed penaltyQuery is a valid
    float value.
    Postcondition: in correct operation, computes what model is optimal
    for the passed penaltyQuery, and gives a boolean signifying its accuracy.
    Both these values are passed back as a struct.
    Exceptions: none?
    Note: none
    */
    std::vector<double> getNewPenaltyList();
    
    /*
    Function name: Minimize
    Algorithm: Acquires a penalty value lambda and returns a minimization result consisting of:
    (the k value corresponding to the selected model, a boolean showing its accuracy)
    Boolean values consist of: F for unsure, and T for a sure pairing. O(log N) query.
    Precondition: for correct operation, the passed penalty is a valid
    float value.
    Postcondition: in correct operation, computes what model is optimal
    for the passed penalty, and gives a boolean signifying its accuracy.
    Both these values are passed back as a struct.
    Exceptions: none?
    Note: none
    */
    MinimizeResult minimize(double penaltyQuery);


    /*UTILITY METHODS*/
    void displayMap();

    void addBreakpoint(Model firstModel, Model secondModel);


    void displayPenList();

    private:
    /*
    Utility function to validate an insertion to delegate it away from the main insert functions.
    The result from std::map insert() is: std::pair<iterator,bool> where the bool represents the success/failure of insertion.
    The function uses this bool value to ensure there is not a duplicate penalty already in the map
    Another case to check for is an invalid penalty value. If the penalty is negative, it is not a valid penalty to insert with a model.*/ 
    std::map<double, Model>::iterator validateInsert(std::pair<std::map<double,Model>::iterator, bool> insertResult);

    //Custom isEmpty method as we will add a initial model, nullifing built-in method with std::map.
    bool hasModelsInserted();  
};

//Utility to compute a breakpoint between two models for use in other functions.
double findBreakpoint(Model firstModel, Model secondModel);
//Computes the penalized cost of k*lambda for breakpoint comparison. 
double findCost(double penalty, int modelSize, int loss);

