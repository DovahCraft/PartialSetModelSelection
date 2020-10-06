#include <map>

#include "test.hpp"
using namespace std;

int main (int argc, char* argv[]){
    //Two keys idea (not really doing what I want yet.)
    /*cout << "DUAL KEY DEMO\n"; 
    runDualKeyDemo();
    cout << "\n\nTWO MAP DEMO\n";
    runTwoMapDemo();*/
    cout << "LOWER BOUND OPERATOR DEMO\n";
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
    ModelSelectionMap modelMap = ModelSelectionMap();
    //Manually set models up with a penalty for each.
    Model model3Segs = Model(4, 2.0);
    model3Segs.penalty = 2.0;
    Model model2Segs = Model(2, 4.0);
    model2Segs.penalty = 4.0;
    Model model1Segs = Model(1, 7.0);
    model1Segs.penalty = 6.0;
    //Insert for testing
    modelMap.insert(model3Segs);
    modelMap.insert(model2Segs);
    modelMap.insert(model1Segs);
    //Display to make sure we have what we want.
    modelMap.displayPathSet();

    auto modelSizeIter = modelMap.lowerBoundModelSize(3);
    int searchedSize = modelSizeIter->modelSize;

    std::cout << "LowerBoundModelSize with 3: " << searchedSize << "\n";

    auto penaltyIter = modelMap.lowerBoundPenalty(3);
    int searchedPen = penaltyIter->penalty;

    std::cout << "LowerBoundModelPenalty with 3: " << searchedPen << "\n";


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
  std::cout <<  "\nModelSize Map Display\n" << "#######################\n";
  std::cout <<  "Penalty" <<  "          " << "ModelSize\n"; 
  for (auto it : path) 
      std::cout << it.penalty << "      ,           " << it.modelSize << '\n';
   std::cout << " \n";
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