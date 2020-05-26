#include <iostream>
#include <map>
#include <gtest/gtest.h>
#include "PartialModelSelection.hpp"
#include <math.h>


//Testing method to test minimize method (minimizeResults).   
void testMinimize(MinimizeResult testResult, double lowModelSize, double highModelSize){
   //Check if the expected range of values is correct.
   EXPECT_EQ(lowModelSize, testResult.modelPair.first);
   EXPECT_EQ(highModelSize, testResult.modelPair.second);


   std::cout<<"Running test for Minimize\n";      
}

//Testing method to test getNextPen
void testGetPen(ModelSelectionMap testMap, double expectedQuery ){
   //Run getNextPen Asserts.
      std::cout<<"Running test for getNextPen\n";


}


//Tests to ensure correct model formation and associated logic.
TEST(ModelTests, modelTest)
   {
    Model model3segs = Model(3, 5);
    ASSERT_EQ(model3segs.model_size, 3);
   }

   
TEST(ModelTests, modelLossTestPos)
   {
    Model model3segs = Model(3, 5);
    ASSERT_EQ(model3segs.loss, 5);

   }

 TEST(TestedPairs, testBreakFormation)
   {
    Model testModel = Model(2, 3);
    double expectedPen = 4.0;
    TestedPair testBP = TestedPair(4.0, testModel);
    ASSERT_EQ(testBP.first, expectedPen);
   }

//Test TestedPair insertion based on panel 1 (left with two models.)
TEST(InsertTests, testInsertLeft)
   {
    ModelSelectionMap testMap = ModelSelectionMap();
    Model model1Seg = Model(1, 7.0);
    Model model2Seg = Model(2, 4.0);

    testGetPen(testMap, 0.0); //Iterator, getNewPenalty iterator? Should give us 0 to inf, so 0 to query. 
    testMap.insert(4.0, model1Seg); 
    testMinimize(testMap.minimize(5.0), 1, 1);
    //Mimize Query here
    testMinimize(testMap.minimize(4.0), 1, 1);
    testMinimize(testMap.minimize(3.0), 1, INFINITY);
    testMap.insert(0.0, model2Seg);
    testMinimize(testMap.minimize(5.0), 1, 1);
    testMinimize(testMap.minimize(4.0), 1, 1);
    //testMinimize(testMap.minimize(3.0), 2, true);
    testMinimize(testMap.minimize(2.0), 2, 2); 
    testMinimize(testMap.minimize(1.0), 2, 2);

   }


//Test TestedPair insertion based on panel 2 (Middle with three models. Low start loss for #3, 2 not considered.)
TEST(InsertTests, testInsertMiddle)
   {
    ModelSelectionMap testMap = ModelSelectionMap();
    Model model1Seg = Model(1, 7.0);
    Model model2Seg = Model(2, 4.0); 
    Model model3Seg = Model(3, 0.0);
    testMap.insert(4.0, model1Seg);
    testMinimize(testMap.minimize(4.0), 1, 1);
    
    testMap.insert(model2Seg); //?????? Never selected so how should I test this? Maybe insert 3 segments after and test the update?
    //Test for path here? Form link list?
    testMap.insert(1.0, model3Seg); //Add model with 3 segments that is found to be more optimal
    testMap.insert(model2Seg);//Non penalty insert for not optimal models. 
    testMap.insert(4.0, model1Seg);
    //Should I remove the 2 segment model from the data structure now?
    ASSERT_EQ(testMap.modelCount, 3);
   }



//Test TestedPair insertion based on panel 2 (Right with three models. Higher start loss for #3, all models considered on path.)
TEST(InsertTests, testInsertRight)
   {
    ModelSelectionMap testMap = ModelSelectionMap();
    Model model1Seg = Model(1, 7.0);
    Model model2Seg = Model(2, 4.0); 
    Model model3Seg = Model(3, 2.0);
    testMap.insert(4.0, model1Seg);
    testMap.insert(2.5, model2Seg);
    testMap.insert(1.0, model3Seg);
    ASSERT_EQ(testMap.modelCount, 3);
   }




//Driver function for google test
int main (int argc, char* argv[]){
    //Init unit tests here
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


