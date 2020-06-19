//System Includes and Libraries
#include <iostream>
#include <map>
#include <gtest/gtest.h>
#include <math.h>
#include <string>

//Local Includes
#include "PartialModelSelection.hpp"

//Macros to display test info in gtest format. 
#define GTEST_MINCOUT std::cerr << "[ MINIMIZE ] [INFO] "
#define GTEST_GETPENCOUT std::cerr << "[ GETPEN ] [INFO] "



//Testing method to test minimize method (minimizeResults).   
void testMinimize(MinimizeResult testResult, double lowModelSize, bool certainFlag, double penaltyQuery){
   //Log the test being run.
   std::string certainFlagString = "";
   
   certainFlag ? certainFlagString = "true" : certainFlagString = "false";
    
   GTEST_MINCOUT << "Running test minimize with parameters: " << "low: " << lowModelSize << 
                     "; certainFlag: " << certainFlagString << "; penalty: " << penaltyQuery << ";\n\n";
   //Check if the expected range of values is correct.
   EXPECT_EQ(lowModelSize, testResult.modelSize) << "\nMinimize first parameter is different from expected.\n";
   EXPECT_EQ(expectedCertainty, testResult.certain) << "\nMinimize certainty flag is different from expected.\n";      

}

//Testing method to test getNextPen
void testGetPen(ModelSelectionMap testMap, double expectedPenalty ){
    GTEST_GETPENCOUT << "Running test for getNextPenalty\n\n";

    EXPECT_EQ(testMap.getNewPenalty(), expectedPenalty) << "Penalty returned by getNewPenalty differs from expected.\n";
    
}


//Tests to ensure correct model formation and associated logic.
TEST(ModelCreationTests, DISABLED_modelTest){
    Model model3segs = Model(3, 5);
    ASSERT_EQ(model3segs.modelSize, 3);
   }

   
TEST(ModelCreationTests, DISABLED_modelLossTestPos){
    Model model3segs = Model(3, 5);
    ASSERT_EQ(model3segs.loss, 5);

   }

 

 TEST(PenaltyPairsTests, DISABLED_testBreakFormation){
    Model testModel = Model(2, 3);
    double expectedPen = 4.0;
    PenaltyModelPair testBP = PenaltyModelPair(4.0, testModel);
    ASSERT_EQ(testBP.first, expectedPen);
   }

   TEST(PenaltyPairsTests, DISABLED_testEmptyMinimization){
    ModelSelectionMap testMap = ModelSelectionMap();
    double expectedStart = 1;
    double expectedEnd = INFINITY;
    MinimizeResult emptyResult = testMap.minimize(4.0);
    ASSERT_EQ(emptyResult.optimalModels.first, expectedStart);
    ASSERT_EQ(emptyResult.optimalModels.second, expectedEnd);
   }

    
   TEST(MinimizeTests, DISABLED_testInitialMinimization){
    ModelSelectionMap testMap = ModelSelectionMap();
    testMinimize(testMap.minimize(0.5), 1, false, 0.5);
   }

   
  TEST(InsertTests, testLowerBoundInsertion){
    ModelSelectionMap testMap = ModelSelectionMap(); //Create a new model selection map class
    Model model3Seg = Model(3, 0.0); //Previous
    Model model2Seg = Model(2, 4.0); //Given with lower_bound
    testMap.insert(2.0, model3Seg); //Insert the three segment model at penalty 3.0.
    auto lowerBeforeInserts = testMap.penaltyModelMap.lower_bound(3.0); //This returns map::end as no key exceeds 3.0
    
    ASSERT_EQ(lowerBeforeInserts, testMap.penaltyModelMap.end()); //Testing that this is indeed the case. 

    ASSERT_EQ(prev(lowerBeforeInserts)->first, 2.0); //Used std::iterator's prev() to revert to the current entry from lower_bound iterator in O(1) time
    testMap.insert(5.0, model2Seg); //Insert a new model with two segments at penalty 5.0
    testMap.displayMap(); 
    auto lowerBAfterInserts = testMap.penaltyModelMap.lower_bound(3.0); //Created a new iterator to the new value returned by lower_bound, iterator points to 5.0 entry.
    ASSERT_EQ(lowerBAfterInserts->first, 5.0); //This lower bound call does indeed return the model stored with key 5.0. THIS IS AFTER THE QUERY, I WANT WHAT IS BEFORE AS WELL.
    auto prevPair = prev(lowerBAfterInserts); //To get what is before the query of 3.0, I must use the prev function on the iterator given by lower_bound
    auto firstPair = prev(lowerBAfterInserts, 2); //To get to the firstPair, I call prev with parameter 2 as well, to get an iterator to the key 0.0, or two keys before. 
    ASSERT_EQ(prevPair->first, 2.0); //Assert that the pair before the lower_bound call is the model we inserted with penalty key 2.0. (the model before).
    ASSERT_EQ(firstPair->first, 0.0); //Assert that the pair two steps before the lower_bound iterator is the first model (key 0).
    //All asserts pass 6/16/20.
  }

   TEST(InsertTests, DISABLED_testDuplicatePenalty){
    ModelSelectionMap testMap = ModelSelectionMap();
    Model model1Seg = Model(1, 7.0);
    int expectedCount = 1;
    testMap.insert(2.0, model1Seg);
    testMap.insert(2.0, model1Seg); //SHould not be allowed by insert function. 
    ASSERT_EQ(expectedCount, testMap.penaltyModelMap.count(2.0));
   
   }



   TEST(DISABLED_InsertTests, testModelSizeAfterUpdate){
     ModelSelectionMap testMap = ModelSelectionMap();
     Model model3Seg = Model(3, 2.0);
     testMap.displayMap();
     Model model2Seg = Model(2, 4.0);
     testMap.insert(1.0, model3Seg);
      testMap.displayMap();
     testMap.insert(3.0, model2Seg);
      testMap.displayMap();
     auto inserted3SegPair = testMap.penaltyModelMap.find(1.0);
     Model testModel = inserted3SegPair->second;
     double expectedSizeAfter = testModel.modelSizeAfter;

     ASSERT_EQ(expectedSizeAfter, model2Seg.modelSize);

   }

 //Test PenaltyModelPair insertion based on panel 1 (left with two models.)
 TEST(InsertTests, testInsertLeftPanel){
    ModelSelectionMap testMap = ModelSelectionMap(testMap.STD_MODEL_CAP);
    Model model1Seg = Model(1, 7.0);
    Model model2Seg = Model(2, 4.0);

    testMinimize(testMap.minimize(5.0), 1, false, 5.0);
   
    testGetPen(testMap, 0.0); //Iterator, getNewPenalty iterator? Should give us 0 to inf, so 0 to query.

    //Insert the one segment model for an incomplete path, given the model cap is 3.  
    testMap.insert(4.0, model1Seg); 
    //Test getNextPenalty
    testGetPen(testMap, 0.0);
    testMinimize(testMap.minimize(5.0), 1, true, 5.0); //This should be certain as nothing will become optimal after it has been solved for a penalty value. 
    //Mimize Query here
    testMinimize(testMap.minimize(4.0), 1, true, 4.0); //We solved this on through the insert.
    testMinimize(testMap.minimize(3.0), 1, false, 3.0); //We only have 1 currently, but more inserts are possible with our cap of three models (which is std).
    testMinimize(testMap.minimize(2.0), 1, false, 2.0);
    testMinimize(testMap.minimize(1.0), 1, false, 1.0);
    testMinimize(testMap.minimize(0.0), 1, false, 0.0);

    //Insert two segment model and test again, should be complete path.  
    testMap.insert(0.0, model2Seg);
    testMinimize(testMap.minimize(5.0), 1, true, 5.0);
    testMinimize(testMap.minimize(4.0), 1, true, 4.0);
    testMinimize(testMap.minimize(3.0), 2, true, 3.0); //Breakpoint is at 3.0, so 2.0 and 1.0 should yield model with two segments as it was inserted with penatly 0.0
    testMinimize(testMap.minimize(2.0), 2, true, 2.0); //These two queries return 2 for sure after we insert it at 0.0, and with the breakpoint at 3.0 computed from the loss and slope.  
    testMinimize(testMap.minimize(1.0), 2, true, 1.0);
    testMinimize(testMap.minimize(1.0), 2, true, 0.0);
   }


//Test PenaltyModelPair insertion based on panel 2 (Middle with three models. Low start loss for #3, 2 not considered.)
TEST(InsertTests, testInsertMiddlePanel){
    ModelSelectionMap testMap = ModelSelectionMap(testMap.STD_MODEL_CAP);
    Model model1Seg = Model(1, 7.0);
    Model model2Seg = Model(2, 4.0); 
    Model model3Seg = Model(3, 0.0);
    bool verboseFlag = false;
    testMap.insert(4.0, model1Seg);
    testMinimize(testMap.minimize(4.0), 1, true, 4.0);
    testMinimize(testMap.minimize(5.0), 1, true, 5.0);
    if(verboseFlag){
      testMinimize(testMap.minimize(3.0), 1, false, 3.0); //We only have 1 currently, but more inserts are possible with our cap of three models (which is std).
      testMinimize(testMap.minimize(2.0), 1, false, 2.0);
      testMinimize(testMap.minimize(1.0), 1, false, 1.0);
      testMinimize(testMap.minimize(0.0), 1, false, 0.0);
    }
    
    //Add model with 2 segments that is found to be less optimal later on as the model cap is still 3. 
    testMap.insert(model2Seg); //Need to compute breakpoint first?
    testMinimize(testMap.minimize(0.0), 1, false, 0.0); //Should give us 1 or 2 for now, but the model cap is 3 so there is a chance that 2 can be overridden/not optimal. 
    //Add model with 3 segments that is found to be more optimal
    testMap.insert(1.0, model3Seg);     
    testMinimize(testMap.minimize(2.0), 3, true, 2.0);
    //testMap.insert(model2Seg);//Non penalty insert for not optimal models. 
   }



//Test PenaltyModelPair insertion based on panel 2 (Right with three models. Higher start loss for #3, all models considered on path.)
//TODO complete this test after the left and middle panel tests pass.
TEST(DISABLED_InsertTests, testInsertRightPanel){
    ModelSelectionMap testMap = ModelSelectionMap(testMap.STD_MODEL_CAP);
    Model model1Seg = Model(1, 7.0);
    Model model2Seg = Model(2, 4.0); 
    Model model3Seg = Model(3, 2.0);
    testMap.insert(4.0, model1Seg);
    testMap.insert(2.5, model2Seg);
    testMap.insert(1.0, model3Seg);
    testGetPen(testMap, 0.0);
   }



//Test PenaltyModelPair insertion based on panel 2 (Right with three models. Higher start loss for #3, all models considered on path.)
TEST(InsertTests, insertSameModelSize){
    ModelSelectionMap testMap = ModelSelectionMap();
    Model model5Seg = Model(5, 1.0);
    testMap.insert(1.0, model5Seg);
    testMinimize(testMap.minimize(1.0), 5, true, 1.0);
    testMap.insert(2.0, model5Seg);
    testMinimize(testMap.minimize(2.0), 5, true, 2.0);
    testMap.insert(3.0, model5Seg);
    testMinimize(testMap.minimize(3.0), 5, true, 3.0);
    
    //This test passes under the current insert implementation, but the memory result is not constant. TODO: Add expanded duplicate key logic to insert!
    testMap.displayMap(); 

   }




//Driver function for google test
int main (int argc, char* argv[]){
    //Init unit tests here
    ::testing::InitGoogleTest(&argc, argv);
    std::cout << "\n##################################################\n" <<  
    "Test suite for partialsetmodelselection.\n Subtests will collapse if successfully ran.\n" 
    << "##################################################\n\n";
    return RUN_ALL_TESTS();
}


