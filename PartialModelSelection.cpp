//System Includes and Libraries
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>
#include <math.h>

//Local includes
#include "PartialModelSelection.hpp"



/*MODEL SELECTION MAP IMPLEMENTATIONS*/

//Default contructor for model selection map, sets max models to std of 3.
ModelSelectionMap::ModelSelectionMap() : maxModels(ModelMapMessages::STD_MODEL_CAP)
{}

//Initialization constructor for a ModelSelectionMap with passed cap value.
ModelSelectionMap::ModelSelectionMap(ModelMapMessages cap) : maxModels(cap) {}


void ModelSelectionMap::insert(double penaltyQuery, Model currentModel)
{
   //Insert into our TestedPair map in the ModelSelectionMap
   TestedPair newTestedPair = TestedPair(penaltyQuery, currentModel);
   testedPairs.insert(newTestedPair);
   modelCount++;
   //Insert into the MinimizeResult Vector to update ranges to test next.
   //std::pair <double,double> optimalModels; 
   //optimalModels = std::make_pair(penaltyQuery, penaltyQuery);
   

      
   //Update the resultList to reflect the current path.
}


void ModelSelectionMap::insert(Model currentModel)
{
   //Runs solver for penaltyQuery to insert
   //std::cout << "Adding model without penaltyQuery \n";

   //Insert pair with created TestedPair from solver.
}

/*
Function name: getNewpenaltyQuery
Algorithm: 
Precondition: for correct operation, the passed penaltyQuery is a valid
   float value.
Postcondition: in correct operation, computes what model is optimal
   for the passed penaltyQuery, and gives a boolean signifying its accuracy.
   Both these values are passed back as a struct.
Exceptions: none?
Note: none
*/
double ModelSelectionMap::getNewPenaltyList()
{

  //If the map of tested pairs is empty, query penaltyQuery 0 first.  
  if(testedPairs.empty())
     {
       return EMPTY_MAP_QUERY; //?
     }

  //TestedPair smallestPair = testedPairs.begin()->first;
  
  for (std::map<double,Model>::iterator it=testedPairs.begin(); it!=testedPairs.end(); ++it)
      std::cout << it->first << " => " << it->second.model_size << '\n';   
      
   return 0;
     
    
  
}


MinimizeResult ModelSelectionMap::minimize(double penaltyQuery){
   //Default result if we do not find a matching penaltyQuery.
   MinimizeResult queryResult = MinimizeResult(); 
   auto indexNode = testedPairs.find(penaltyQuery);

   //If we have no inserted model/penaltyQuery pairs, return the default result from 0 to inf.
   if(testedPairs.empty()){
    std::cout << "Map is empty, returning default min result\n";
    return queryResult;
   }

    //Otherwise, we need to find a penaltyQuery result that is closest to the queried penaltyQuery
    std::cout << "Couldn't find tested pair, used lower_bound\n";
    indexNode = testedPairs.lower_bound(penaltyQuery);

    if(indexNode->first == penaltyQuery)
      queryResult = MinimizeResult(std::make_pair(indexNode->second.model_size,indexNode->second.model_size));
   
   //Otherwise, make a range query that is unsure. 
    else{


    }
    
    return queryResult;
}



//Method to display the currently stored pairs in the map. 
void ModelSelectionMap::displayMap() {
   
  for (std::map<double,Model>::iterator it=testedPairs.begin(); it!=testedPairs.end(); ++it)
      std::cout << it->first << " => " << it->second.model_size << '\n';

}



//MinimizeResult Method Implementations

//MinimizeResult default constructor to be used when no models have been inserted. 
MinimizeResult::MinimizeResult(){
 //Initialize a default result, which has 1 selected, but unsure.
 optimalModels.first = -1;
 optimalModels.second = INFINITY;
 certain = false;

}

MinimizeResult::MinimizeResult(std::pair<double, double> inputModels)
   {
       //Check for a valid range of models. 
       if(!isValidRange(inputModels))
          {
           throw std::out_of_range("Invalid range inputted to MinimizeResult creation."); 
          }

       else
          {
           //Check if our penaltyQueryRange is a solved point (Where the beginning and end are identical.)
           if(optimalModels.first == optimalModels.second)
              {
               certain = true;
              }
            else{
               certain = false;  

            }
            optimalModels.first = inputModels.first;
            optimalModels.second = inputModels.second;

          }
       
      
   }


bool MinimizeResult::isValidRange(std::pair<double,double> optimalModels)
   {
    //Tie a model size and pairing to a penaltyQuery range.
    if(optimalModels.first < 0 || optimalModels.first < optimalModels.second)
       {
        return false;   
       }
    return true;
   }



std::pair<int, int> ModelSelectionMap::solver(double penaltyQuery)
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

