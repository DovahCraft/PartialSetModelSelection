//System Includes and Libraries
#include <iostream>
#include <map>
#include <stdexcept>
#include <list>
#include <math.h>
#include <string>
#include <iterator>
#include <algorithm>

//Local includes
#include "PartialModelSelection.hpp"

/*MODEL SELECTION MAP IMPLEMENTATIONS*/
ModelSelectionMap::ModelSelectionMap(double maxModels) : modelSizeCap(maxModels) {   
   //Set a starting point to be stored at penalty 0 using a placeholder pair to return default results. 
   Model startingModel = Model(1,1);
   startingModel.optimalPenalties = std::make_pair(0,0);
   startingModel.modelSizeAfter = 1;
   startingModel.isPlaceHolder = true;
   startingModel.isCertainPairing = false;
   PenaltyModelPair startingPair = PenaltyModelPair(0.0, startingModel);
   penaltyModelMap.insert(startingPair);
   lastInsertedPair = penaltyModelMap.end(); //Set the previous pair to the end of the map as placeholder does not count. 
   insertedModels = 0; //This starts at 0 as we exclude the beginning placeholder. 
   newPenaltyList.push_back(0.0);
}

void ModelSelectionMap::insert(double newPenalty, int modelSize, double loss){
   //Insert into ourpenaltyModelPair map in the ModelSelectionMap if the newPenalty is not within it.
   Model newModel = Model(modelSize, loss);
   PenaltyModelPair newPair = PenaltyModelPair(newPenalty, newModel);
   std::map<double,Model>::iterator nextPair = penaltyModelMap.lower_bound(newPenalty);
   std::map<double, Model>::iterator prevPair = prev(nextPair);
   //By default, we have the same model size after us, unless it is updated below.
   newPair.second.modelSizeAfter = modelSize;
   try{
       validateInsert(newPair, nextPair); //Will throw a logic_error exception if duplicate keys are found, handled below.
       //Since we passed validation and are using the 3 parameter method, set our pairing to certain.
       newPair.second.isCertainPairing = true;
       penaltyModelMap.insert(newPair);
       //Update initial placeholder pair       
       if(insertedModels == 0){
          auto placeHolder = penaltyModelMap.begin();
          placeHolder->second.modelSize = newModel.modelSize;
      } 
       insertedModels++;
       //UPDATE MODEL BEFORE US 
       //If we found another key besides the 0 key from lowerbound. 
       if(nextPair->first != 0.0){
          prevPair->second.modelSizeAfter = newModel.modelSize;
          if(prevPair->second.isPlaceHolder) prevPair->second.modelSize = modelSize;
          //Add a breakpoint to the candidatePenaltiesList.
          addBreakpoint(newModel, prevPair->second);
       }
       //UPDATE MODELS AFTER US
       if(nextPair != penaltyModelMap.end()){
          newPair.second.modelSizeAfter = nextPair->second.modelSize;
          //Add a breakpoint with the next model to the candidatePenaltiesList
          addBreakpoint(newModel, nextPair->second); 
       }
       //If there is nothing different after us, set the after value to the current value. 
       else{
          newPair.second.modelSizeAfter = newModel.modelSize;
       }
      //Update the last inserted pair iterator
      lastInsertedPair = penaltyModelMap.find(newPair.first);  
   }
   catch(std::logic_error errorMessage) {
      //Update if the first pairing is a placeholder, error if it's not. 
      auto firstPair = penaltyModelMap.begin();
      if(newPenalty == 0 && firstPair->second.isPlaceHolder){
         firstPair->second.modelSize = newModel.modelSize;
         firstPair->second.modelSizeAfter = newModel.modelSize;
         firstPair->second.loss = newModel.loss;
         firstPair->second.isCertainPairing = true;
         std::cout << "Insert of penalty 0 found, updating placeholder value!\n";
         firstPair->second.isPlaceHolder = false;
         //Since we found 0 with lower bound, get the next highest pairing to find breakpoint with. 
         nextPair = penaltyModelMap.upper_bound(newPenalty);
         addBreakpoint(prevPair->second, nextPair->second);
         //As we officially solved a model for 0, increment the inserted models as it is no longer a placeholder. 
         insertedModels++;
         //Update the last inserted pair iterator
         lastInsertedPair = penaltyModelMap.find(newPair.first);
      }
      //We already inserted a model here for sure, error out.   
      else{
         std::cerr << errorMessage.what() << "\n";
      }  
   }
}

void ModelSelectionMap::insert(int modelSize, double loss){
   Model newModel = Model(modelSize, loss);
   //This breakpoint will be computed and compared to the previous breakpoint stored, if any.
   double candidateBkpt = -1;

   if(lastInsertedPair != penaltyModelMap.end()){
      int lastModelSize = lastInsertedPair->second.modelSize;
      candidateBkpt = findBreakpoint(newModel, lastInsertedPair->second);
      insert(candidateBkpt, modelSize, loss); //This will update the last inserted pair, so we need to use the variable above
      lastInsertedPair->second.modelSizeAfter = lastModelSize;
   }
   //If there is nothing in there, add at penalty INFINITY as we don't know anything about other models to chain to. 
   else{
      //Call the insert function and update the 0.0 placeholder to reflect insertion
      insert(INFINITY, modelSize, loss);
   }
}

void ModelSelectionMap::remove(std::map<double, Model>::iterator toRemove){
   //Remove from map
   penaltyModelMap.erase(toRemove);
   //Update other entries to reflect new path (May need to be before removal 
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
    if(indexPenalty == penaltyQuery) {
      //Make a query result to return using the second element of a testedPair, Model. Get its modelSize.
      if(indexModel.isCertainPairing)
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
       if(prevModel.modelSize == modelSizeCap) isCertain = true;
       
       queryResult = MinimizeResult(prevModel.modelSize, isCertain); //GDB reveals this is called with prevModel.modelSize when it shouldn't be.nb
    } 
    return queryResult;
}

void ModelSelectionMap::addBreakpoint(Model firstModel, Model secondModel){
   int modelSizeDiff = abs(firstModel.modelSize - secondModel.modelSize);
   double newBreakpoint;
   //We need to check for an improperly formed breakpoint within findBreakpoint (it would be less than 0).
   try{
      newBreakpoint = findBreakpoint(firstModel,secondModel);
      if(firstModel.modelSize != secondModel.modelSize)
         newPenaltyList.push_back(newBreakpoint);
   } 
   //If we have an invalid breakpoint, don't add it and display an error instead.
   catch(std::logic_error error){
      error.what();
   }
   if(modelSizeDiff == 1){
      std::cout << "Difference between model: " << firstModel.modelSize << "and model: " << secondModel.modelSize << " is one!\n";
      //TODO: Leverage this to establish breakpoint surity. Perhaps remodel the path based on breakpoints gathered from insertions? 
   }
}
 
std::map<double, Model>::iterator ModelSelectionMap::validateInsert(PenaltyModelPair newPair, std::map<double, Model>::iterator nextPair){
    double candidatePenalty = newPair.first;
    auto prevPair = prev(nextPair);
    int attemptedInsertSize = nextPair->second.modelSize;
    int existingModelSize = nextPair->second.modelSize;

    //If we already have a model inserted at the desired penalty
    if(newPair.first == nextPair->first){
      //Get existing pair for error code
      throw std::logic_error("[ ERROR ] Cannot insert model_size = " + std::to_string(attemptedInsertSize) 
        + " because model_size = " + std::to_string(existingModelSize) + " already exists at penalty = " 
           + std::to_string(candidatePenalty) + "\n");
    }  

    //If the candidate penalty for insertion is less than the min value of 0, throw an error.
    if(candidatePenalty < 0){
      throw std::logic_error("[ ERROR ] Cannot insert with penalty = " + std::to_string(candidatePenalty) +
         " because it is less than 0!");
    }

    //Check if the new penalty is redundant, as it is within an already established range. 
    if(!prevPair->second.isPlaceHolder && prevPair->second.modelSize == attemptedInsertSize 
               && nextPair->second.modelSize == attemptedInsertSize){
      throw std::logic_error("[ ERROR ] Cannot insert at penalty = " + std::to_string(candidatePenalty) 
           + " because it lies within the solved range of [" + std::to_string(prevPair->first) 
                + "," + std::to_string(nextPair->first) + "].\n");
    }
    //TODO: Add condition to widen the range on an insertion. 
    return nextPair;
}

//General Utilities used in testing and in the ModelSelectionMap class. 
double ModelSelectionMap::getNewPenalty(){
   double newPenalty = newPenaltyList.front();
   newPenaltyList.pop_front();
   return newPenalty;
}

bool ModelSelectionMap::hasModelsInserted(){return insertedModels > 0;}

//DISPLAY METHODS
void ModelSelectionMap::displayMap() {
  std::cout <<  "\nCurrent Map Display\n" << "#######################\n";
  std::cout << "Penalty          ModelSize          ModelSizeAfter\n"; 
  for (std::map<double,Model>::iterator it=penaltyModelMap.begin(); it!=penaltyModelMap.end(); ++it) 
      std::cout << it->first << "      =>           " << it->second.modelSize << "      =>           "  << it->second.modelSizeAfter << '\n';

   std::cout << " \n";
 }
void ModelSelectionMap::displayPenList(){
    std::cout << "Candidate penalties in newPenList: " << "\n";
    for (std::list<double>::iterator it=newPenaltyList.begin(); it!=newPenaltyList.end(); ++it)
      std::cout << *it << "   ";
   std::cout << " \n";
 }
  
//MinimizeResult Method Implementations
//Initialization constructor for a minimizeResult based on passed args from the minimize method. 
MinimizeResult::MinimizeResult(int inputModelSize, bool inputCertainty) : modelSize(inputModelSize), certain(inputCertainty){}

//Breakpoint computation method 
double findBreakpoint(Model firstModel, Model secondModel){
   double newBreakpoint = (secondModel.loss - firstModel.loss) / (firstModel.modelSize - secondModel.modelSize);
   if(newBreakpoint < 0){
      throw std::logic_error("[ ERROR ] Breakpoint computed between Modelsize: " + std::to_string(firstModel.modelSize) + "and Modelsize: "+ std::to_string(secondModel.modelSize) 
                        + "is less than zero, and is therefore invalid.");
   }
   return newBreakpoint;
}

double findCost( double penalty, int modelSize, double loss){return penalty*modelSize + loss;}














