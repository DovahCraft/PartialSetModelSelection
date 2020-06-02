//System Includes and Libraries
#include <iostream>
#include <map>
#include <gtest/gtest.h>
#include <math.h>

//Local Includes
#include "PartialModelSelection.hpp"

//Macros to display test info in gtest format. 
#define GTEST_MINCOUT std::cerr << "[ MINIMIZE ] [INFO] "
#define GTEST_GETPENCOUT std::cerr << "[ GETPEN ] [INFO] "



//Testing method to test minimize method (minimizeResults).   
void testMinimize(MinimizeResult testResult, double lowModelSize, bool certainFlag, double penaltyQuery){
   //Log the test being run.
   GTEST_MINCOUT << "Running test for minimize with: " << "low: " << lowModelSize << 
                     "; certain: " << certainFlag << "; penalty: " << penaltyQuery << ";\n\n";
   //Check if the expected range of values is correct.
   EXPECT_EQ(lowModelSize, testResult.optimalModels.first) << "\nMinimize first parameter is different from expected.\n";
   EXPECT_EQ(certainFlag, testResult.certain) << "\nMinimize certainty flag is different from expected.\n";      
}

//Testing method to test getNextPen
void testGetPen(ModelSelectionMap testMap, double expectedPenalty ){
    GTEST_GETPENCOUT << "Running test for getNextPenalty\n\n";

    EXPECT_EQ(testMap.getNewPenaltyList(), expectedPenalty) << "Penalty returned by getNewPenalty differs from expected.\n";
    
}


//Tests to ensure correct model formation and associated logic.
TEST(ModelTests, modelTest){
    Model model3segs = Model(3, 5);
    ASSERT_EQ(model3segs.model_size, 3);
   }

   
TEST(ModelTests, modelLossTestPos){
    Model model3segs = Model(3, 5);
    ASSERT_EQ(model3segs.loss, 5);

   }

 TEST(TestedPairs, testBreakFormation){
    Model testModel = Model(2, 3);
    double expectedPen = 4.0;
    TestedPair testBP = TestedPair(4.0, testModel);
    ASSERT_EQ(testBP.first, expectedPen);
   }

   TEST(TestedPairs, testEmptyMinimization){
    ModelSelectionMap testMap = ModelSelectionMap();
    double expectedStart = -1;
    double expectedEnd = INFINITY;
    MinimizeResult emptyResult = testMap.minimize(4.0);
    ASSERT_EQ(emptyResult.optimalModels.first, expectedStart);
    ASSERT_EQ(emptyResult.optimalModels.second, expectedEnd);
   }

 //Test TestedPair insertion based on panel 1 (left with two models.)
 TEST(InsertTests, testInsertLeftPanel){
    ModelSelectionMap testMap = ModelSelectionMap(testMap.STD_MODEL_CAP);
    Model model1Seg = Model(1, 7.0);
    Model model2Seg = Model(2, 4.0);

    testMinimize(testMap.minimize(5.0), -1, INFINITY, 5.0);
   
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


//Test TestedPair insertion based on panel 2 (Middle with three models. Low start loss for #3, 2 not considered.)
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



//Test TestedPair insertion based on panel 2 (Right with three models. Higher start loss for #3, all models considered on path.)
TEST(InsertTests, testInsertRightPanel){
    ModelSelectionMap testMap = ModelSelectionMap(testMap.STD_MODEL_CAP);
    Model model1Seg = Model(1, 7.0);
    Model model2Seg = Model(2, 4.0); 
    Model model3Seg = Model(3, 2.0);
    testMap.insert(4.0, model1Seg);
    testMap.insert(2.5, model2Seg);
    testMap.insert(1.0, model3Seg);
    testGetPen(testMap, 0.0);
   }



//Test TestedPair insertion based on panel 2 (Right with three models. Higher start loss for #3, all models considered on path.)
TEST(InsertTests, insertSameModelSize){
    ModelSelectionMap testMap = ModelSelectionMap();
    Model model5Seg = Model(5, 1.0);
    testMap.insert(1.0, model5Seg);
    testMinimize(testMap.minimize(1.0), 5, true, 1.0);
    testMap.insert(2.0, model5Seg);
    testMinimize(testMap.minimize(2.0), 5, true, 2.0);
    testMap.insert(3.0, model5Seg);
    testMinimize(testMap.minimize(3.0), 5, true, 3.0);
    
    //This test passes under the current insert implementation, but the memory result is not constant. 
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


