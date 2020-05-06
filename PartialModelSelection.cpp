#include <iostream>
#include <map>

#include "PartialModelSelection.hpp"

int main (void){
    //Unit tests here
    std::cout << "Hello world!\n";
    
    Model model3segs = Model(3, 5);
    minimize(5.44);
    solver(5.34);
      
    ModelSelectionMap testMap = ModelSelectionMap();
    //Call minimizer
    //Call insert
    testMap.insert(3.5, model3segs);
}
//Default contructor for model selection map
ModelSelectionMap::ModelSelectionMap(): maxModels(ModelCapMessages::STD_MODEL_CAP){}

//Initialization constructor for a ModelSelectionMap with a cap.
ModelSelectionMap::ModelSelectionMap(ModelCapMessages cap) : maxModels(cap){}

void ModelSelectionMap::insert(double penalty, Model currentModel){
    std::cout << "Calling insert on current test map.\n";
    
}


/*
Function name: insert
Algorithm: Inserts a new model into our partial set (map) data structure
Precondition: The model is formatted correctly and the
   penalty is a valid float?
Postcondition: Inserts the model into the data structure.
   No return type, maybe return success code eventually.
Exceptions: correctly and appropriately (without program failure)
    responds to and reports failure to insert the model.
Note: none
*/
void insert(double penalty, Model currentModel, ModelSelectionMap currentMap)
   {
    currentMap.insert(penalty,currentModel);
    std::cout << "Calling insertsdfasdfdsvf! \n";
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
    std::cout << "Testing minimize\n";
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