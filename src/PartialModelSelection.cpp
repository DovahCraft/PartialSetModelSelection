//System Includes and Libraries
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>
#include <math.h>
#include <string>
//Local includes
#include "PartialModelSelection.hpp"
#include "ModelSelectionUtils.hpp"


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
      std::cout << "Insert failed, validation returned an error!\n";
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
      std::cout << "Insert failed, validation returned an error!\n";
   }
   
   
   
   insertedModels = 0; //This starts at 0 as we exclude the beginning placeholder.
}


void ModelSelectionMap::insert(double newPenalty, Model newModel){
   //Insert into ourpenaltyModelPair map in the ModelSelectionMap if the newPenalty is not within it.
   PenaltyModelPair newPair = PenaltyModelPair(newPenalty, newModel);
   
   penaltyModelMap.insert(newPair);

   //After inserting, update our previous entry as well. 
   updatePreviousEntry();
   
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



std::pair<int, int> ModelSelectionMap::solver(double penaltyQuery)
{
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

 void ModelSelectionMap::updatePreviousEntry(){
   std::cout << "Updating previous entry based on new insert!\n";
}


 //General Utilities used in ModelSelectionMap

//Takes in an insertion result and returns the iterator to the insertion if it is valid. 
std::map<double, Model>::iterator validateInsert(std::pair<std::map<double, Model>::iterator, bool> insertResult){
    //The result from insert is: std::pair<iterator,bool> where the bool represents the success/failure of insertion.
    if(!insertResult.second) throw std::logic_error("Invalid insert, key exists."); 
    auto validIterator = insertResult.first;
    return validIterator;
}














