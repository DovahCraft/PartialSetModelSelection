//System Includes and Libraries
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>
#include <math.h>

//Local includes
#include "PartialModelSelection.hpp"
#include "ModelSelectionUtils.hpp"

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
