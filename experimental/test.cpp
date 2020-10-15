#include "test.hpp"
using namespace std;

int main (int argc, char* argv[]){
    runLowerBoundsDemo();
}

void ModelSelectionMap::insert(Model toInsert){
  path.insert(toInsert);
}

std::set<Model>::iterator ModelSelectionMap::lowerBoundModelSize(int modelSizeQuery){
  Model queryModel = Model(modelSizeQuery, 0);
  queryModel.penalty = PENALTY_CONSTANT;
  auto iterator = path.lower_bound(queryModel);
  return iterator;
}

std::set<Model>::iterator ModelSelectionMap::lowerBoundPenalty(double penaltyQuery){
  Model queryModel = Model(MODELSIZE_CONSTANT, 0);
  queryModel.penalty = penaltyQuery;
  auto iterator = path.lower_bound(queryModel);
  return iterator;
}

//Std set with two lower_bound methods supporting it.
void runLowerBoundsDemo(){
    std::cout << "\nCOLLECTION OF DEMOS FOR THE TWO LOWER BOUND METHODS.\n\n";

    //Demo 1
    std::cout << "\n[DEMO1] General Testing of Lower bound methods\n\n";
    ModelSelectionMap modelMap = ModelSelectionMap();
    //Manually set models up with a penalty for each.
    Model model8Segs = Model(8, 0.0);
    model8Segs.penalty = 0.0;
    Model model6Segs = Model(6, 1.0);
    model6Segs.penalty = 1.0;
    Model model4Segs = Model(4, 2.0);
    model4Segs.penalty = 2.0;
    //Model model2Segs = Model(2, 4.0);
    //model2Segs.penalty = 4.0;
    //Model model1Segs = Model(1, 7.0);
    //model1Segs.penalty = 6.0;
    //Insert for testing
    modelMap.insert(model8Segs);
    modelMap.insert(model6Segs);
    //modelMap.insert(model4Segs);
   // modelMap.insert(model2Segs);
   // modelMap.insert(model1Segs);
    //Display to make sure we have what we want.
    modelMap.displayPathSet();

    int querySize = 6;
    auto modelSizeIter = modelMap.lowerBoundModelSize(querySize);
    int outputSize = modelSizeIter->modelSize;

    std::cout << "LowerBound with query ModelSize " << querySize <<" : " << outputSize << "\n";

    int queryPen  = 1;
    auto penaltyIter = modelMap.lowerBoundPenalty(queryPen);
    double outputPen = penaltyIter->penalty;

    std::cout << "LowerBoundModelPenalty with query penalty " << queryPen << " : " << outputPen << "\n";


    //Demo 2 Based off of middle panel (low starting loss for 3segs)
    std::cout << "\n[DEMO2] Middle panel scenario\n\n";
    ModelSelectionMap modelMap2 = ModelSelectionMap();
    Model model1Seg = Model(1,7.0);
    model1Seg.penalty = INFINITY;
    Model model2Seg = Model(2,4.0);
    Model model3Seg = Model(3,0.0);

    modelMap2.insert(model1Seg);
    //Trying to insert model with two segments
    auto it = modelMap2.lowerBoundModelSize(2);
    Model queriedModel = *it;
    double candidate1_2 = findBreakpoint(model2Seg, queriedModel);

    //Breakpoint between model 1 and model 2: 3.0
    std::cout << "Searched breakpoint between model1_2 : " << candidate1_2 << " \n";
    if(candidate1_2 >= queriedModel.penalty)
      std::cout << "Need to remove searched model, the new insertion is more optimal! \n";
    else{
      model2Seg.penalty = candidate1_2;
    }
    modelMap2.insert(model2Seg);
    
    //Will point us to the two segment model
    auto it2 = modelMap2.lowerBoundModelSize(3);
    Model outputModel = *it2;

    //This will cause model2 to be removed due to Algorithm 2. 
    double candidate2_3 = findBreakpoint(model3Seg, outputModel);
    std::cout << "Candidate 2_3 BREAKPOINT : " << candidate2_3 << " \n";

    if(candidate2_3 >= outputModel.penalty){
      std::cout << "Need to remove searched model, the new insertion is more optimal! \n";
    }

    //Demo 3 Based off of the right panel (high starting loss for 3segs)
    std::cout << "\n[DEMO3] Right panel scenario\n\n";
    ModelSelectionMap modelMap3 = ModelSelectionMap();
    //Increase the 3 segment model's loss so it does not invalidate model 2.
    model3Seg.loss = 2.0;

    //Start by inserting model1 again. 
    modelMap3.insert(model1Seg);
    //First lowerbound connecting model 1 and 2
    auto it3 = modelMap2.lowerBoundModelSize(2);
    Model queriedModel2 = *it;
    model2Seg.penalty = findBreakpoint(model2Seg, queriedModel);

    //Breakpoint between model 1 and model 2: 3.0
    std::cout << "SEARCHED MODEL2SEG BREAKPOINT : " << model2Seg.penalty << " \n";
    modelMap3.insert(model2Seg);
    
    //Will point us to the two segment model
    auto it4 = modelMap2.lowerBoundModelSize(3);
    Model outputModel2 = *it2;

    model3Seg.penalty = findBreakpoint(model3Seg, outputModel);
    std::cout << "SEARCHED MODEL3SEG BREAKPOINT : " << model3Seg.penalty << " \n";
    modelMap3.insert(model3Seg);

    modelMap3.displayPathSet();

}


void runDualKeyDemo(){
    map<pair<double, int>, Model> modelMap;

    Model model3Segs = Model(3, 2.0);
    Model model2Segs = Model(2, 4.0);
    Model model1Segs = Model(1, 7.0);
    modelMap.insert(make_pair(make_pair(0.0, 3), model3Segs));
    modelMap.insert(make_pair(make_pair(4.0, 2), model2Segs));
    modelMap.insert(make_pair(make_pair(7.0, 1), model1Segs));

    //SEcond parameter is irrelevant currently, comparisons are only being done on the penalty, which the first element of the pair. 
    auto lowerBound3 = modelMap.lower_bound(make_pair(0.0, 2));
    auto lowerBound2 = modelMap.lower_bound(make_pair(3.0, 1));
    auto lowerBound1 = modelMap.lower_bound(make_pair(4.5, 1));

    //Yields map.end() as nothing is greater than or equal to penalty 8.0
    auto lowerBoundEnd = modelMap.lower_bound(make_pair(8.0, 1));  
    
    displayDualMap(modelMap);

    //3
    cout << "Lower bound with 0.0,2: " <<  lowerBound3->first.second << " \n";
    //2
    cout << "Lower bound with 3.0, 1: " << lowerBound2->first.second << " \n";
    //1
    cout << "Lower bound with 4.5, 1: " << lowerBound1->first.second << " \n";
    //0 (map.end())
    cout << "Lower bound with 7.0, 1: " << lowerBoundEnd->first.second << " \n";
}

void runTwoMapDemo(){
  //Our two maps
  map<int, Model> modelSizeMap;
  map<double, Model> penaltyMap;

  //Models to insert
  Model model6Segs = Model(6, 2.0);
  Model model2Segs = Model(2, 4.0);
  Model model1Segs = Model(1, 7.0);

  //Insert for ModelSize
  modelSizeMap.insert(make_pair(6,model6Segs));
  penaltyMap.insert(make_pair(0,model6Segs));
  modelSizeMap.insert(make_pair(2,model2Segs));
  penaltyMap.insert(make_pair(4.0, model2Segs));
  modelSizeMap.insert(make_pair(1,model1Segs));
  penaltyMap.insert(make_pair(5.0, model1Segs));

  //Inserts for Penalty
  modelSizeMap.insert(make_pair(6,model6Segs));
  modelSizeMap.insert(make_pair(2,model2Segs));
  modelSizeMap.insert(make_pair(1,model1Segs));

  displayModelSizeMap(modelSizeMap);
  displayPenaltyMap(penaltyMap);

  auto result1 = modelSizeMap.lower_bound(2);
  auto result2 = modelSizeMap.lower_bound(3);

  cout << "Lower_bound with 2: " << result1->first << "\n";
  cout << "Lower_bound with 3: " << result2->first << "\n";
}


void ModelSelectionMap::displayPathSet() {
  std::cout <<  "\nPathSet Display\n" << "#######################\n";
  std::cout <<  "Penalty" <<  "          " << "ModelSize\n"; 
  for (auto it : path) 
      std::cout << it.penalty << "      ,           " << it.modelSize << '\n';
   std::cout << " \n";
 }

//Breakpoint computation method 
double findBreakpoint(Model firstModel, Model secondModel){
   double newBreakpoint = (secondModel.loss - firstModel.loss) / (firstModel.modelSize - secondModel.modelSize);
   if(newBreakpoint < 0){
      throw std::logic_error("[ ERROR ] Breakpoint computed between Modelsize: " + std::to_string(firstModel.modelSize) + "and Modelsize: "+ std::to_string(secondModel.modelSize) 
                        + "is less than zero, and is therefore invalid.");
   }
   return newBreakpoint;
}


void displayDualMap(map<pair<double, int>, Model> modelMap) {
  std::cout <<  "\nDualMap Display\n" << "#######################\n"; 
  for (std::map<std::pair<double, int>,Model>::iterator it=modelMap.begin(); it!=modelMap.end(); ++it) 
      std::cout << it->first.first << "      ,           " << it->first.second << '\n';

   std::cout << " \n";
 }

 void displayModelSizeMap(map<int, Model> modelMap) {
  std::cout <<  "\nModelSize Map Display\n" << "#######################\n"; 
  for (auto it=modelMap.begin(); it!=modelMap.end(); ++it) 
      std::cout << it->first << "      ,           " << it->second.modelSize << '\n';

   std::cout << " \n";
 }

 void displayPenaltyMap(map<double, Model> modelMap) {
  std::cout <<  "\nPenalty Map Display\n" << "#######################\n"; 
  for (auto it=modelMap.begin(); it!=modelMap.end(); ++it) 
      std::cout << it->first << "      ,           " << it->second.modelSize << '\n';

   std::cout << " \n";
 }