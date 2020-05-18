#include <iostream>
#include <map>
#include "PartialModelSelection.hpp"


/*MODEL SELECTION MAP METHODS*/
//Default contructor for model selection map
ModelSelectionMap::ModelSelectionMap(): maxModels(ModelCapMessages::STD_MODEL_CAP){}

//Initialization constructor for a ModelSelectionMap with a cap.
ModelSelectionMap::ModelSelectionMap(ModelCapMessages cap) : maxModels(cap){}

/*
Function name: insert
Algorithm: Inserts a new model into our partial set (map) data structure with a penalty modeling FPOP
Precondition: The model is formatted correctly and the
   penalty is a valid double.
Postcondition: Inserts the model into the data structure.
   No return type, maybe return success code eventually.
Exceptions: correctly and appropriately (without program failure)
    responds to and reports failure to insert the model.
Note: none
*/ 
void ModelSelectionMap::insert(double penalty, Model currentModel){
    std::cout << "Calling insert on current test map.\n";
    
}


/*
Function name: insert
Algorithm: Inserts a new model into our partial set (map) without a penalty, modeling 
Precondition: The model is formatted correctly and the
   penalty is a valid double.
Postcondition: Inserts the model into the data structure.
   No return type, maybe return success code eventually.
Exceptions: correctly and appropriately (without program failure)
    responds to and reports failure to insert the model.
Note: none
*/ 
void ModelSelectionMap::insert(Model currentModel)
{

}






/*
Function name: Minimize
Algorithm: Acquires a penalty value lambda and returns a tuple consisting of:
   (the k value corresponding to the selected model, a boolean showing its accuracy)
   Boolean values consist of: F for unsure, and T for a sure pairing.
Precondition: for correct operation, the passed penalty is a valid
   float value.
Postcondition: in correct operation, computes what model is optimal
   for the passed penalty, and gives a boolean signifying its accuracy.
   Both these values are passed back as a tuple.
Exceptions: correctly and appropriately (without program failure)
    responds to and reports incorrectly formatted op code data,
Note: none
*/
std::pair<int, bool> minimize(double penalty)
{
    auto tempPair = std::make_pair<int, bool>(4, true);
    //Temporary stub return
    return tempPair;
}

/*
Function name: Solver
Algorithm:

Precondition: for correct operation, the passed penalty is a valid
   float value.
Postcondition:
Exceptions: correctly and appropriately (without program failure)
    responds to and reports incorrectly formatted op code data,
Note: none
*/
std::pair<int, int> solver(double penalty)
{
    auto tempPair = std::make_pair<int, bool>(4, true); 
    std::cout << "Testing solver!\n";
    //Temporary stub return
    return tempPair;
}

