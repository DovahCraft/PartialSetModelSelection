#include <iostream>
#include <map>
#include "PartialModelSelection.hpp"
#include <stdbool.h>
#include <stdexcept>
#include <vector>
#include <math.h>


/*MODEL SELECTION MAP IMPLEMENTATIONS*/

//Default contructor for model selection map, sets max models to std of 3.
ModelSelectionMap::ModelSelectionMap() : maxModels(ModelMapMessages::STD_MODEL_CAP)
{}

//Initialization constructor for a ModelSelectionMap with passed cap value.
ModelSelectionMap::ModelSelectionMap(ModelMapMessages cap) : maxModels(cap) {}

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
void ModelSelectionMap::insert(double penalty, Model currentModel)
{
   //Insert into our TestedPair map in the ModelSelectionMap
   TestedPair newTestedPair = TestedPair(penalty, currentModel);
   testedPairs.insert(newTestedPair);
   modelCount++;
   //Insert into the MinimizeResult Vector to update ranges to test next.
   //std::pair <double,double> optimalModels; 
   //optimalModels = std::make_pair(penalty, penalty);
   

      
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
   //std::cout << "Adding model without penalty \n";

   //Insert pair with created TestedPair from solver.
}

/*
Function name: getNewPenalty
Algorithm: 
Precondition: for correct operation, the passed penalty is a valid
   float value.
Postcondition: in correct operation, computes what model is optimal
   for the passed penalty, and gives a boolean signifying its accuracy.
   Both these values are passed back as a struct.
Exceptions: none?
Note: none
*/
double ModelSelectionMap::getNewPenaltyList()
{

  //If the map of tested pairs is empty, query penalty 0 first.  
  if(testedPairs.empty())
     {
       return EMPTY_MAP_QUERY; //?
     }

  //TestedPair smallestPair = testedPairs.begin()->first;
  
  for (std::map<double,Model>::iterator it=testedPairs.begin(); it!=testedPairs.end(); ++it)
      std::cout << it->first << " => " << it->second.model_size << '\n';   
      
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
Exceptions: none?
Note: none
*/
MinimizeResult ModelSelectionMap::minimize(double penalty){
   //Default result if we do not find a matching penalty.
   MinimizeResult queryResult = MinimizeResult();
   std::map<double, Model>::iterator indexNode; 
   indexNode = testedPairs.find(penalty);

   //If we have no inserted model/penalty pairs, return the default result from 0 to inf.
   if(testedPairs.empty()){
    std::cout << "Map is empty, returning default min result\n";
    return queryResult;
   }

   //If we find the penalty in the map, then we have a certain query result
   if(indexNode != testedPairs.end()){
       //ISSUE: Using the penalty value here instead of model sizes. 
       queryResult = MinimizeResult(std::make_pair(indexNode->second.model_size,indexNode->second.model_size),indexNode->second.model_size);  
       return queryResult;

   } 
   
    

   //Temporary stub return
   return queryResult;
}



//Method to display the currently stored pairs in the map. 
void ModelSelectionMap::displayMap() {
   
  for (std::map<double,Model>::iterator it=testedPairs.begin(); it!=testedPairs.end(); ++it)
      std::cout << it->first << " => " << it->second.model_size << '\n';

}
  
//MinimizeResult Methods

//MinimizeResult default constructor to be used when no models have been inserted. 
MinimizeResult::MinimizeResult(){
 //Initialize a default result, which has 1 selected, but unsure.
 optimalModels.first = -1;
 optimalModels.second = INFINITY;
 certain = false;

}

MinimizeResult::MinimizeResult(std::pair<double, double> inputModels, int model_size)
   {
       //Check for a valid range of models. 
       if(!isValidRange(inputModels))
          {
           throw "Invalid range inputted to MinimizeResult creation.";
          }

       else
          {
           //Check if our penaltyRange is a solved point (Where the beginning and end are identical.)
           if(optimalModels.first == optimalModels.second)
              {
               certain = true;
              }
            optimalModels.first = inputModels.first;
            optimalModels.second = inputModels.second;

          }
       
      
   }


bool MinimizeResult::isValidRange(std::pair<double,double> optimalModels)
   {
    //Tie a model size and pairing to a penalty range.
    if(optimalModels.first < 0 || optimalModels.first < optimalModels.second)
       {
        return false;   
       }
    return true;
   }


/*
Function name: Solver
Algorithm:

Precondition: for correct operation, the passed penalty is a valid
   float value.
Postcondition:
Exceptions: none yet. 
Note: none
*/
std::pair<int, int> ModelSelectionMap::solver(double penalty)
{
   auto tempPair = std::make_pair<int, bool>(4, true);
   std::cout << "Testing solver!\n";
   //Temporary stub return
   return tempPair;
}



//Basic getters/setters
int ModelSelectionMap::getModelCount()
{
 return modelCount;

}

