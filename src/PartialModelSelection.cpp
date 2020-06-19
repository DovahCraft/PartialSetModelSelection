//System Includes and Libraries
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>
#include <math.h>
#include <string>
//Local includes
#include "PartialModelSelection.hpp"



/*MODEL SELECTION MAP IMPLEMENTATIONS*/

//Default contructor for model selection map, sets max models to std of 3.
ModelSelectionMap::ModelSelectionMap() : maxModels(STD_MODEL_CAP){
   //Set a starting point to be stored at penalty 0 using a placeholder pair to return default results. 
   Model startingModel = Model(1,PLACEHOLDER_LOSS);
   startingModel.modelSizeAfter = 1;
   
   startingModel.isPlaceHolder = true;
   PenaltyModelPair startingPair = PenaltyModelPair(0.0, startingModel);
   penaltyModelMap.insert(startingPair);
   insertedModels = 0; //This starts at 0 as we exclude the beginning placeholder. 
}

//Initialization constructor for a ModelSelectionMap with passed cap value.
ModelSelectionMap::ModelSelectionMap(int maxModels) : maxModels(maxModels) {   
   //Set a starting point to be stored at penalty 0 using a placeholder pair to return default results. 
   Model startingModel = Model(1,PLACEHOLDER_LOSS);
   startingModel.modelSizeAfter = 1;
   startingModel.isPlaceHolder = true;
   PenaltyModelPair startingPair = PenaltyModelPair(0.0, startingModel);
   penaltyModelMap.insert(startingPair);
   insertedModels = 0; //This starts at 0 as we exclude the beginning placeholder. 
}

void ModelSelectionMap::insert(double newPenalty, Model newModel){
   //Insert into ourpenaltyModelPair map in the ModelSelectionMap if the newPenalty is not within it.
   PenaltyModelPair newPair = PenaltyModelPair(newPenalty, newModel);
   auto nextPair = penaltyModelMap.lower_bound(newPenalty);
   std::map<double, Model>::iterator prevPair;
   try{
      auto insertResult = penaltyModelMap.insert(newPair);
      validateInsert(insertResult); //Will throw a logic_error exception if duplicate keys are found, handled below.       
      if(insertedModels == 0){
         auto placeHolder = penaltyModelMap.begin();
         placeHolder->second.modelSize = newModel.modelSize;
      }
      //Note that we inserted a model. 
      insertedModels++;
      //UPDATE MODELS AFTER US
      //If we found a model/penalty pairing that is higher than our current query
      if(nextPair != penaltyModelMap.end())
         newPair.second.modelSizeAfter = nextPair->second.modelSize;
      //If there is nothing different after us, set the after value to the current value. 
      else{
         newPair.second.modelSizeAfter = newModel.modelSize;
      }
      //UPDATE MODELS BEFORE US
      //If we found another key besides the 0 key from lowerbound. 
      if(nextPair->first != 0.0){
         prevPair = prev(nextPair);
         prevPair->second.modelSizeAfter = newModel.modelSize;
      }
   }

   catch(std::logic_error errorMessage) {
      //update the placeholder on value instead of returning an error message if penalty is 0.
      auto firstKey = penaltyModelMap.begin();
      if(newPenalty == 0 && firstKey->second.isPlaceHolder){
         firstKey->second.modelSize = newModel.modelSize;
         firstKey->second.modelSizeAfter = newModel.modelSize;
         std::cout << "Insert of penalty 0 found, updating placeholder value!\n";
         firstKey->second.isPlaceHolder = false;
         //As we official solved a model for 0, increment the inserted models as it is no longer a placeholder. 
         insertedModels++;
      }
         
      else{
         std::cout << "Insert failed, key exists and is not the initial placeholder!\n";
      }
      
   }


}

void ModelSelectionMap::insert(Model currentModel){};

MinimizeResult ModelSelectionMap::minimize(double penaltyQuery){
   //Default result if we do not find a matching penaltyQuery.
   MinimizeResult queryResult = MinimizeResult(); 
   auto indexPair = penaltyModelMap.lower_bound(penaltyQuery);
   double indexPenalty = indexPair->first;
   Model indexModel = indexPair->second;
   auto prevPair = prev(indexPair);
   Model prevModel = prevPair->second;

    //If we found an inserted pair that lies on the queried penalty itself
    if(indexPenalty == penaltyQuery) {
       //Make a query result to return using the second element of a testedPair, Model. Get its modelSize. 
       queryResult = MinimizeResult(indexModel.modelSize, true);
    }


    //If we find a result that lies after an inserted 1 segment model, it should 1 for sure.  
    else if(indexPair == penaltyModelMap.end() && prevModel.modelSize == 1 && !prevModel.isPlaceHolder){
       queryResult = MinimizeResult(prevModel.modelSize, true);
    } 

    else{
       //If we find a result that is not zero, but is not a solved point for sure. TODO: updated logic here with breakpoints and model cap bounds. 
       if(indexPair->first != 0){
          auto prevPair = prev(indexPair);
          Model prevModel = prevPair->second;
          queryResult = MinimizeResult(prevModel.modelSize, false); 
       }
    } 

    //Return the processed query to the user.
    return queryResult;
}

double ModelSelectionMap::getNewPenalty(){
  //If the map of tested pairs is empty, query penaltyQuery 0 first.  
  if(!hasModelsInserted()){
       return EMPTY_MAP_QUERY; //?
  }
   return 0; 
}

std::pair<int, int> ModelSelectionMap::solver(double penaltyQuery){
   auto tempPair = std::make_pair<int, bool>(4, true);
   std::cout << "Testing solver!\n";
   //Temporary stub return
   return tempPair;
}

//Method to display the currently stored pairs in the map. 
void ModelSelectionMap::displayMap() {
  std::cout <<  "\nCurrent Map Display\n" << "#######################\n";
  std::cout << "Penalty          ModelSize          ModelSizeAfter\n"; 
  for (std::map<double,Model>::iterator it=penaltyModelMap.begin(); it!=penaltyModelMap.end(); ++it)
      std::cout << it->first << "      =>           " << it->second.modelSize << "      =>           "  << it->second.modelSizeAfter << '\n';

   std::cout << " \n";
 }
  
 bool ModelSelectionMap::hasModelsInserted(){
    //Check to see if only the default entry is in the map.
    return insertedModels > 0;

 }




//General Utilities used in ModelSelectionMap
//Takes in an insertion result and returns the iterator to the insertion if it is valid. 
std::map<double, Model>::iterator validateInsert(std::pair<std::map<double, Model>::iterator, bool> insertResult){
    //The result from insert is: std::pair<iterator,bool> where the bool represents the success/failure of insertion.
    if(!insertResult.second) throw std::logic_error("Invalid insert, key exists."); 
    auto validIterator = insertResult.first;
    return validIterator;
}


//MinimizeResult Method Implementations
//MinimizeResult default constructor to be used when no models have been inserted. 
MinimizeResult::MinimizeResult(){
 //Initialize a default result, which has 1 selected, but unsure.
 optimalModels.first = 1;
 optimalModels.second = INFINITY;
 certain = false;

}

//Initialization constructor for a minimizeResult based on passed args from the minimize method. 
MinimizeResult::MinimizeResult(int inputModelSize, bool inputCertainty) : modelSize(inputModelSize), certain(inputCertainty){}














