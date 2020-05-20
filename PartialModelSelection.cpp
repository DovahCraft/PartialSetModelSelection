#include <iostream>
#include <map>
#include "PartialModelSelection.hpp"
#include <stdbool.h>


/*MODEL SELECTION MAP IMPLEMENTATIONS*/

//Default contructor for model selection map, sets max models to std of 3.
ModelSelectionMap::ModelSelectionMap(): maxModels(ModelCapMessages::STD_MODEL_CAP){
   //Init head of minimizeResult LL
   //resultList->head = NULL;

}

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
    //Insert into our breakpoint map in the ModelSelectionMap
    Breakpoint newBreakPoint = Breakpoint(penalty, currentModel);
    breakpoints.insert(newBreakPoint);
    modelCount++;

    //Insert a minimized result based on what we know.

    //Update the resultList to reflect the current path. 
    

}


/*
Function name: insert
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
void ModelSelectionMap::insert(Model currentModel)
{
     //Runs solver for penalty to insert
     std::cout<< "Adding model without penalty \n";

     //Insert pair with created breakpoint from solver. 
}


/*
Function name: getNewPenalty
Algorithm: Acquires a penalty value lambda and returns a minimization result consisting of:
   (the k value corresponding to the selected model, a boolean showing its accuracy)
   Boolean values consist of: F for unsure, and T for a sure pairing.
Precondition: for correct operation, the passed penalty is a valid
   float value.
Postcondition: in correct operation, computes what model is optimal
   for the passed penalty, and gives a boolean signifying its accuracy.
   Both these values are passed back as a struct.
Exceptions: correctly and appropriately (without program failure)
    responds to and reports incorrectly formatted op code data,
Note: none
*/
double ModelSelectionMap::getNewPenalty()
   {

    MinimizeResult *currentResult = resultList->head;  
    //Search through the minimizeResult list, seeing where we have "certain" values to query next with solver.
    //while(currentResult != NULL) {
       //Traverse and check for useful penalty ranges to query. 

    //}
    return 0;
   }



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
Exceptions: correctly and appropriately (without program failure)
    responds to and reports incorrectly formatted op code data,
Note: none
*/
MinimizeResult ModelSelectionMap::minimize(double penalty)
{
   MinimizeResult queryResult;
   //if(!resultList)
    
    //Temporary stub return
    return queryResult;
}


//Add minimize result function for LL?
void ModelSelectionMap::addResult(MinimizeResult *toAdd)
{
  std::cout << "Adding MinimizeResult to list\n";
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

