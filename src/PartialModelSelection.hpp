#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <list>

//Struct to embody Model,Boolean pairs for model selection path records.
struct MinimizeResult {
    MinimizeResult(int modelSize = 1, bool certain = false);
    bool certain;
    int modelSize;
};

struct Model {
    //Create a model if it has valid modelSize and loss values given. 
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
    //Used to store the next model size, changes from this.modelSize if there are two models at a breakpoint
    int modelSizeAfter; 
    //Used to determine if the key at 0 is the initial key we insert.
    bool isPlaceHolder;
    //Used to determine if the current penalty/model pairing should be considered certain. 
    bool isCertainPairing; 
    //Used to filter out unnecessary penalties within optimal range. (NOT YET USED)
    std::pair<double,double> optimalPenalties;  
};

//struct penaltyModelPair may be better here for more readability
using PenaltyModelPair = std::pair<double, Model>;

class ModelSelectionMap {
public:
    //This is used to determine if the map is 'empty' as the initial model inserted scews isEmpty() counts.
    int insertedModels; 

    //Max model size permitted to be inserted into the map. Defaults to INFINITY (no limit).
    const double modelSizeCap;

    std::map<double,Model>::iterator lastInsertedPair; //Holds the previously computed breakpoint, if it exists, for use in constant time insertion.

    std::map<double,Model>::iterator largestSelected;

    //Map struct to hold penalty and model pairings from inserts.
    std::map<double, Model> penaltyModelMap; 
    
    //List to hold new candidate penalties and breakpoints to give new information from minimize.
    std::list<double> newPenaltyList;
    
    //Method headers
    //Default value of INFINITY for no passed cap
    ModelSelectionMap(double maxModels = INFINITY);
    /*
     Function name(s): insert
     Algorithm: Inserts a new model into our partial set (map) data structure with a penalty modeling FPOP.
     The penalty is inserted using the provided parameter, indicating that the model created from the modelSize 
     and loss parameters is optimal at that penalty key, for sure. 
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
    binary segmentation and other constrained style solvers. Forms a model struct 
    using the passed in modelSize and loss parameters. The penalty is computed using a breakpoint.
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
    Exceptions: correctly and appropriately (without program failure)
        responds to and reports failure to insert the model.
    Note: none
    */
    void remove(std::map<double, Model>::iterator toRemove);


    /*
    Function name: getNewPenalty
    Algorithm: O(1) or O(log N) query of a penalty value that will result in new information. 
    Precondition: None
    Postcondition: Returns a double value 
    Exceptions: none?
    Note: none
    */
    double getNewPenalty();
    
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


    /*UTILITY METHODS WITHIN MODELSELECTIONMAP*/
    
    //Simply displays the current map structure
    void displayMap();

    //Adds a new breakpoint penalty to the back of the newPenalties list for later consideration. 
    void addBreakpoint(Model firstModel, Model secondModel);

    //Displays the list of penalties currently stored to recommend for a new query in getNewPenalty.
    void displayPenList();

    private:
    /*
    Utility function to validate an insertion to delegate it away from the main insert functions.
    The result from std::map insert() is: std::pair<iterator,bool> where the bool represents the success/failure of insertion.
    The function uses this bool value to ensure there is not a duplicate penalty already in the map
    Another case to check for is an invalid penalty value. If the penalty is negative, it is not a valid penalty to insert with a model.*/ 
    std::map<double, Model>::iterator validateInsert(PenaltyModelPair newPair, std::map<double,Model>::iterator nextPair);

    //Custom isEmpty method as we will add a initial model, nullifing built-in method with std::map.
    bool hasModelsInserted();  
};

/*GENERAL COMPUTATIONS USED IN TESTS AND MAP*/

//Utility to compute a breakpoint between two models for use in other functions.
double findBreakpoint(Model firstModel, Model secondModel);
//Computes the penalized cost of k*lambda for breakpoint comparison. 
double findCost(double penalty, int modelSize, int loss);

