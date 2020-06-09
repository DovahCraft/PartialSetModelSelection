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
ModelSelectionMap::ModelSelectionMap() : maxModels(STD_MODEL_CAP){
   //Set a starting point to be stored at penalty 0 using placeholders to be updated. 
   Model startingModel = Model(1,PLACEHOLDER_LOSS);
   startingModel.modelSizeAfter = 1;
   PenaltyModelPair startingPair = PenaltyModelPair(0.0, startingModel); 
   penaltyModelMap.insert(startingPair);
}

//Initialization constructor for a ModelSelectionMap with passed cap value.
ModelSelectionMap::ModelSelectionMap(int maxModels) : maxModels(maxModels) {   
   Model startingModel = Model(1,PLACEHOLDER_LOSS);
   startingModel.modelSizeAfter = 1;
   PenaltyModelPair startingPair = PenaltyModelPair(0.0, startingModel); 
   penaltyModelMap.insert(startingPair);
}


void ModelSelectionMap::insert(double newPenalty, Model newModel)
{
   if(penaltyModelMap.count(newPenalty))
      return;

    //Insert into ourpenaltyModelPair map in the ModelSelectionMap if the newPenalty is not within it.
    auto mapIterator = penaltyModelMap.lower_bound(newPenalty);

    PenaltyModelPair newPair = PenaltyModelPair(newPenalty, newModel);
    penaltyModelMap.insert(newPair);     


   
   


}


void ModelSelectionMap::insert(Model currentModel)
{
   //Runs solver for penaltyQuery to insert
   //std::cout << "Adding model without penaltyQuery \n";

   //Insert pair with createdpenaltyModelPair from solver.
}



MinimizeResult ModelSelectionMap::minimize(double penaltyQuery){
   //Default result if we do not find a matching penaltyQuery.
   MinimizeResult queryResult = MinimizeResult(); 
   auto indexPair = penaltyModelMap.find(penaltyQuery);

   //If we have no inserted model/penaltyQuery pairs, return the default result from 0 to inf.
   if(penaltyModelMap.empty()){
    std::cout << "Map is empty, returning default min result\n";
    return queryResult;
   }

    //If we have models inserted, we need to find a penaltyQuery result that is closest to the queried penaltyQuery
    indexPair = penaltyModelMap.lower_bound(penaltyQuery);

    if(indexPair->first == penaltyQuery)
      //Make a query result to return using the second element of a testedPair, Model. Get it's model_size. 
      queryResult = MinimizeResult(std::make_pair(indexPair->second.model_size,indexPair->second.model_size));
   
    //Otherwise, make a range query that is unsure. 
    else{
       queryResult = MinimizeResult(std::make_pair(indexPair->second.model_size, indexPair->second.modelSizeAfter)); //Returning default for now. 
       queryResult.certain = false;
    }
    
    return queryResult;
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
  if(penaltyModelMap.empty())
     {
       return EMPTY_MAP_QUERY; //?
     }

  //penaltyModelpenaltyModelPair smallestPair = penaltyModelMap.begin()->first;
  
  //for (std::map<double,Model>::iterator it=penaltyModelMap.begin(); it!=penaltyModelMap.end(); ++it)
    //  std::cout << it->first << " => " << it->second.model_size << '\n';   
   return 0;
     
    
  
}

//Method to display the currently stored pairs in the map. 
void ModelSelectionMap::displayMap() {
  std::cout << "#################################\n" << "Current Map Display\n" << "#################################\n"; 
  for (std::map<double,Model>::iterator it=penaltyModelMap.begin(); it!=penaltyModelMap.end(); ++it)
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




