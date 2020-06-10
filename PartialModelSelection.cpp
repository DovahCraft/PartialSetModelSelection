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

   insertedModels = 0; //This starts at 0 as we exclude the beginning placeholder. 
}

//Initialization constructor for a ModelSelectionMap with passed cap value.
ModelSelectionMap::ModelSelectionMap(int maxModels) : maxModels(maxModels) {   
   Model startingModel = Model(1,PLACEHOLDER_LOSS);
   startingModel.modelSizeAfter = 1;
   PenaltyModelPair startingPair = PenaltyModelPair(0.0, startingModel); 
   penaltyModelMap.insert(startingPair);

   insertedModels = 0; //This starts at 0 as we exclude the beginning placeholder.
}


void ModelSelectionMap::insert(double newPenalty, Model newModel)
{
   bool verboseFlag = true; //Used for testing to toggle additional error msgs. 

   if(penaltyModelMap.count(newPenalty))
      //TODO: update existing penalties if needed. 
      return;

    //Insert into ourpenaltyModelPair map in the ModelSelectionMap if the newPenalty is not within it.
    auto indexPair = penaltyModelMap.lower_bound(newPenalty);
    if(verboseFlag)
    std::cout << "Current model size inserting: " << newModel.modelSize << "Index model size: " << indexPair->second.modelSize << "\n";

    

    //If we find a previous entry, update its modelSizeAfter field with the size of the new model we are inserting.
    if(indexPair != penaltyModelMap.end()){
      double indexPenalty = indexPair->first;
      Model indexModel = indexPair->second;
      indexModel.modelSizeAfter = newModel.modelSize;
      std::cout << "Inserted: " << newModel.modelSize << "into the modelSizeAfter field of model: " << indexModel.modelSize << "\n"; 
    }
    
    PenaltyModelPair newPair = PenaltyModelPair(newPenalty, newModel);
    penaltyModelMap.insert(newPair);
    insertedModels++;     


   
   


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
   double indexPenalty = indexPair->first;
   Model indexModel = indexPair->second;

   //If we have no inserted model/penaltyQuery pairs, return the default result from 0 to inf.
   if(!hasModelsInserted()){
    std::cout << "Map is empty, returning default min result\n";
    return queryResult;
   }

    //If we have models inserted, we need to find a penaltyQuery result that is closest to the queried penaltyQuery
    indexPair = penaltyModelMap.lower_bound(penaltyQuery);

    //If we found an inserted pair that lies on the queried penalty itself
    if(indexPenalty == penaltyQuery)
      //Make a query result to return using the second element of a testedPair, Model. Get it's modelSize. 
      queryResult = MinimizeResult(std::make_pair(indexModel.modelSize,indexModel.modelSize));
   
    //Otherwise, make a range query that does not lie on an inserted pair. 
    else{
       queryResult = MinimizeResult(std::make_pair(indexModel.modelSize, indexModel.modelSizeAfter)); //Returning default for now. 
       queryResult.certain = false;
    }
    
    return queryResult;
}




double ModelSelectionMap::getNewPenaltyList()
{

  //If the map of tested pairs is empty, query penaltyQuery 0 first.  
  if(!hasModelsInserted())
     {
       return EMPTY_MAP_QUERY; //?
     }

  //penaltyModelpenaltyModelPair smallestPair = penaltyModelMap.begin()->first;
  
  //for (std::map<double,Model>::iterator it=penaltyModelMap.begin(); it!=penaltyModelMap.end(); ++it)
    //  std::cout << it->first << " => " << it->second.modelSize << '\n';   
   return 0;
     
    
  
}

//Method to display the currently stored pairs in the map. 
void ModelSelectionMap::displayMap() {
  std::cout <<  "\nCurrent Map Display\n" << "#######################\n";
  std::cout << "Penalty          ModelSize\n"; 
  for (std::map<double,Model>::iterator it=penaltyModelMap.begin(); it!=penaltyModelMap.end(); ++it)
      std::cout << it->first << "      =>           " << it->second.modelSize << '\n';

   std::cout << " \n";
 }
  


 bool ModelSelectionMap::hasModelsInserted(){
    //Check to see if only the default entry is in the map.
    return insertedModels > 0;

 }


//MinimizeResult Method Implementations

//MinimizeResult default constructor to be used when no models have been inserted. 
MinimizeResult::MinimizeResult(){
 //Initialize a default result, which has 1 selected, but unsure.
 optimalModels.first = 1;
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




