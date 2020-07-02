//System Includes and Libraries
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>
#include <math.h>
#include <string>
#include <iterator>
#include <algorithm>
//Local includes
#include "PartialModelSelection.hpp"



/*MODEL SELECTION MAP IMPLEMENTATIONS*/
ModelSelectionMap::ModelSelectionMap(double maxModels) : modelSizeCap(maxModels) {   
   //Set a starting point to be stored at penalty 0 using a placeholder pair to return default results. 
   Model startingModel = Model(1,-1); //Used -1 as a default starting loss, will probably refactor.
   startingModel.modelSizeAfter = 1;
   startingModel.isPlaceHolder = true;
   PenaltyModelPair startingPair = PenaltyModelPair(0.0, startingModel);
   penaltyModelMap.insert(startingPair);
   lastInsertedPair = penaltyModelMap.end(); //Set the previous pair to the end of the map as placeholder does not count. 
   insertedModels = 0; //This starts at 0 as we exclude the beginning placeholder. 
   newPenalties.push_back(0.0);
}

void ModelSelectionMap::insert(double newPenalty, int modelSize, double loss){
   //Insert into ourpenaltyModelPair map in the ModelSelectionMap if the newPenalty is not within it.
   Model newModel = Model(modelSize, loss);
   PenaltyModelPair newPair = PenaltyModelPair(newPenalty, newModel);
   auto nextPair = penaltyModelMap.lower_bound(newPenalty);
   std::map<double, Model>::iterator prevPair;

   try{
      auto insertResult = penaltyModelMap.insert(newPair);
      validateInsert(insertResult); //Will throw a logic_error exception if duplicate keys are found, handled below.
      //Update initial placeholder pair       
      if(insertedModels == 0){
         std::map<double, Model>::iterator placeHolder = penaltyModelMap.begin();
         placeHolder->second.modelSize = newModel.modelSize;
      } 
      insertedModels++;
      //UPDATE MODELS AFTER US
      if(nextPair != penaltyModelMap.end())
         newPair.second.modelSizeAfter = nextPair->second.modelSize;
      //If there is nothing different after us, set the after value to the current value. 
      else{
         newPair.second.modelSizeAfter = newModel.modelSize;
      }
      //UPDATE MODEL BEFORE US 
      //If we found another key besides the 0 key from lowerbound. 
      if(nextPair->first != 0.0){
         prevPair = prev(nextPair);
         prevPair->second.modelSizeAfter = newModel.modelSize;
      }

      //Update the last inserted pair iterator
      lastInsertedPair = penaltyModelMap.find(newPair.first);
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

void ModelSelectionMap::insert(int modelSize, double loss){
   Model newModel = Model(modelSize, loss);
   double candidateBkpt = -1;

   if(lastInsertedPair != penaltyModelMap.end()){
      candidateBkpt = findBreakpoint(newModel, lastInsertedPair->second);
   }

}

MinimizeResult ModelSelectionMap::minimize(double penaltyQuery){
   //Default result if we do not find a matching penaltyQuery.
   MinimizeResult queryResult{}; 
   auto indexPair = penaltyModelMap.lower_bound(penaltyQuery);
   double indexPenalty = indexPair->first;
   Model indexModel = indexPair->second;
   auto prevPair = prev(indexPair);
   Model prevModel = prevPair->second;
   bool isCertain = false;

    //If we found an inserted pair that lies on the queried penalty itself
    if(indexPenalty == penaltyQuery && !indexModel.isPlaceHolder) {
       //Make a query result to return using the second element of a testedPair, Model. Get its modelSize.
       isCertain = true; 
       queryResult = MinimizeResult(indexModel.modelSize, isCertain);
    }
    //If we find a result that lies after an inserted 1 segment model, it should 1 for sure.  End of map clause.
    else if(indexPair == penaltyModelMap.end() && prevModel.modelSize == 1 && !prevModel.isPlaceHolder){
       isCertain = true;
       queryResult = MinimizeResult(prevModel.modelSize, isCertain);
    } 

    //If we find a result that is not after 1, nor is it a solved point for sure. TODO: updated logic here with breakpoints and model cap bounds.
    else{
       //If we are below the final model size alloted, then the result is certain. 
       if(prevModel.modelSize == modelSizeCap){
          isCertain = true;
       } 
       queryResult = MinimizeResult(prevModel.modelSize, isCertain);
    } 
    //Return the processed query.
    return queryResult;
}

std::vector<double> ModelSelectionMap::getNewPenaltyList(){
  //Return the list of potential penalties to query next.   
   return newPenalties; 
}

double findBreakpoint(Model firstModel, Model secondModel){
   return (secondModel.loss - firstModel.loss) / (firstModel.modelSize - secondModel.modelSize);
}

//Method to display the currently stored pairs in the map. 
void ModelSelectionMap::displayMap() {
  std::cout <<  "\nCurrent Map Display\n" << "#######################\n";
  std::cout << "Penalty          ModelSize          ModelSizeAfter\n"; 
  for (std::map<double,Model>::iterator it=penaltyModelMap.begin(); it!=penaltyModelMap.end(); ++it)
      std::cout << it->first << "      =>           " << it->second.modelSize << "      =>           "  << it->second.modelSizeAfter << '\n';

   std::cout << " \n";
 }

 void ModelSelectionMap::displayPenList(){
    std::cout << "Candidate penalties in newPenList: " << "\n";
    for (std::vector<double>::iterator it=newPenalties.begin(); it!=newPenalties.end(); ++it)
      std::cout << *it << "   ";
   
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


//Initialization constructor for a minimizeResult based on passed args from the minimize method. 
MinimizeResult::MinimizeResult(int inputModelSize, bool inputCertainty) : modelSize(inputModelSize), certain(inputCertainty){}














