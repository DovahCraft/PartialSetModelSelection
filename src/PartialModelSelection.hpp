#pragma once
#include <iostream>
#include <map>
#include <vector>

struct Model {
    Model(int modelSize, double loss) : modelSize(modelSize), loss(loss) {}
    //Number of segments (k-value)
    int modelSize = 0;
    //Loss associated with the given model 
    double loss = 0.0;
    int modelSizeAfter; //Used for next Model (the after flag in psuedocode)
    bool isSameAfter; //Used to determine if the current modelSize is the same as the predicted next.
    bool isPlaceHolder; //Used to determine if the key at 0 is the initial key we insert.
    //Range of penalties for which this model is optimal.
    std::pair<double,double> optimalPenaltyRange;
};


//Struct to embody Model,Boolean pairs for model selection path records.
struct MinimizeResult {
    MinimizeResult();
    MinimizeResult(std::pair<double,double> inputModelRange);
    bool isValidRange(std::pair<double,double> inputModelRange);
    bool certain = false;
    //Stores the potential models that could encompass a penalty query. Identical first and second value if certain (solved).
    std::pair<double, double> optimalModels;
};



//struct penaltyModelPair may be better here for more readability
using PenaltyModelPair = std::pair<double, Model>;

class ModelSelectionMap {
public:
    //Map constants and return codes.
    const double PLACEHOLDER_LOSS = -9999.0;
    const double EMPTY_MAP_QUERY = 0;
    const double EMPTY_MAP_ERR = -99999;
    const double DEFAULT_PENALTY = -9999;   
    const int STD_MODEL_CAP = 3;
    int insertedModels; //This is used to determine if the map is 'empty' as the initial model inserted scews isEmpty() counts.
    int maxModels;
    
 
    //Map struct to hold penalty and model pairings from inserts.
    std::map<double, Model> penaltyModelMap; 

    std::map<double,Model>::iterator prevInsertedPair = penaltyModelMap.end();  //Pointer to the last inserted pair for updates.

    //Method headers
    ModelSelectionMap();

    ModelSelectionMap(int maxModels);


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
    double getNewPenalty();

    


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

    
    //Wrapper struct to contain a list of MinimizeResults
    std::vector<MinimizeResult> minResultVec; 

    /*UTILITY METHODS*/
    void displayMap();


    private:
        bool hasModelsInserted(); //Custom isEmpty method as we will add a initial model, nullifing built-in method.     
};

//Utility function to validate an insertion, used before setting previous penaltyModel Pair inserted. 
std::map<double, Model>::iterator validateInsert(std::pair<std::map<double,Model>::iterator, bool> insertResult);

