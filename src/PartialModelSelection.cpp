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
   //Set a starting point to be stored at penalty 0 using placeholders to be updated. 
   Model startingModel = Model(1,PLACEHOLDER_LOSS);
   startingModel.modelSizeAfter = 1;
   PenaltyModelPair startingPair = PenaltyModelPair(0.0, startingModel);
   try{
      auto insertResult = penaltyModelMap.insert(startingPair);
      validateInsert(insertResult);
      prevInsertedPair = insertResult.first; //Set the iterator to the previous insert to the validated insert as we did not error.
      
   }
   catch(std::logic_error errorMessage) {
      std::cout << "Insert failed, key exists!\n";
   }
   insertedModels = 0; //This starts at 0 as we exclude the beginning placeholder. 
}

//Initialization constructor for a ModelSelectionMap with passed cap value.
ModelSelectionMap::ModelSelectionMap(int maxModels) : maxModels(maxModels) {   
   Model startingModel = Model(1,PLACEHOLDER_LOSS);
   startingModel.modelSizeAfter = 1;
   PenaltyModelPair startingPair = PenaltyModelPair(0.0, startingModel); 
   try{
      auto insertResult = penaltyModelMap.insert(startingPair);
      validateInsert(insertResult);
      prevInsertedPair = insertResult.first; //Set the iterator to the previous insert to the validated insert as we did not error.  
   }
   catch(std::logic_error errorMessage) {
      std::cout << "Insert failed, key exists!\n";
   }
   insertedModels = 0; //This starts at 0 as we exclude the beginning placeholder.
}

void ModelSelectionMap::insert(double newPenalty, Model newModel){
   //Insert into ourpenaltyModelPair map in the ModelSelectionMap if the newPenalty is not within it.
   PenaltyModelPair newPair = PenaltyModelPair(newPenalty, newModel);
   auto nextHighestKey = penaltyModelMap.lower_bound(newPenalty);
   std::map<double, Model>::iterator prevKey;
   //If we found a model/penalty pairing that is higher than our current query
   if(nextHighestKey != penaltyModelMap.end())
      newPair.second.modelSizeAfter = nextHighestKey->second.modelSize;

   //If the 
   if(nextHighestKey->first != 0.0){
      prevKey = prev(nextHighestKey);
   }

   try{
      auto insertResult = penaltyModelMap.insert(newPair);
      validateInsert(insertResult);
      //After inserting, update our previous entry as well. 
      prevInsertedPair = insertResult.first; //Set the iterator to the previous insert to the validated insert as we did not error.

      //Note that we inserted a model. 
      insertedModels++;

   //Update the model before us.    
   }
   catch(std::logic_error errorMessage) {
      //update the placeholder on value instead of returning an error message if penalty is 0.
      if(newPenalty == 0){
         auto placeHolder = penaltyModelMap.find(0.0);
         placeHolder->second = newModel; 
      }
         

      std::cout << "Insert failed, key exists!\n";
      return;
   }



   //If we have a key before us, update that key's modelSizeAfter to our newly inserted modelsize. 
   //if(prevKey != nullptr)
      //prevKey->second.modelSizeAfter = newPair.second.modelSize;
   
   


}

void ModelSelectionMap::insert(Model currentModel){};

MinimizeResult ModelSelectionMap::minimize(double penaltyQuery){
   //Default result if we do not find a matching penaltyQuery.
   MinimizeResult queryResult = MinimizeResult(); 
   auto indexPair = penaltyModelMap.find(penaltyQuery);
   double indexPenalty = indexPair->first;
   Model indexModel = indexPair->second;

    //If we have models inserted, we need to find a penaltyQuery result that is closest to the queried penaltyQuery
    indexPair = penaltyModelMap.lower_bound(penaltyQuery);

    //If we found an inserted pair that lies on the queried penalty itself
    if(indexPenalty == penaltyQuery)
    //Make a query result to return using the second element of a testedPair, Model. Get its modelSize. 
    queryResult = MinimizeResult(std::make_pair(indexModel.modelSize,indexModel.modelSize));
   
    //Otherwise, make a range query that does not lie on an inserted pair. 
    else{
       queryResult = MinimizeResult(std::make_pair(indexModel.modelSize, indexModel.modelSizeAfter)); //Returning default for now. 
       queryResult.certain = false;
    }
    
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
  std::cout << "Penalty          ModelSize\n"; 
  for (std::map<double,Model>::iterator it=penaltyModelMap.begin(); it!=penaltyModelMap.end(); ++it)
      std::cout << it->first << "      =>           " << it->second.modelSize << '\n';

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

MinimizeResult::MinimizeResult(std::pair<double, double> inputModels){
       //Check for a valid range of models. 
       if(!isValidRange(inputModels)) {
           throw std::out_of_range("Invalid range inputted to MinimizeResult creation."); 
      }
       else {
           //Check if our penaltyQueryRange is a solved point (Where the beginning and end are identical.)
           if(optimalModels.first == optimalModels.second)
               certain = true;
            else{
               certain = false;  
            }
            optimalModels.first = inputModels.first;
            optimalModels.second = inputModels.second;
      }
   }

bool MinimizeResult::isValidRange(std::pair<double,double> optimalModels){
    //Tie a model size and pairing to a penaltyQuery range.
    if(optimalModels.first < 0 || optimalModels.first < optimalModels.second){
        return false;   
    }
    return true;
}













