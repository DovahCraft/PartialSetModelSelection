#include <iostream>
#include <map>
#include <gtest/gtest.h>
#include "PartialModelSelection.hpp"

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

 TEST(Breakpoints, testBreakFormation)
   {
    Model testModel = Model(2, 3);
    double expectedPen = 4.0;
    Breakpoint testBP = Breakpoint(4.0, testModel);
    ASSERT_EQ(testBP.first, expectedPen);
   }

//Test breakpoint insertion based on panel 1 (left with two models.)
TEST(InsertTests, testInsertLeft)
   {
    ModelSelectionMap testMap = ModelSelectionMap();
    Model model1Seg = Model(1, 7.0);
    Model model2Seg = Model(2, 4.0);
    testMap.insert(4.0, model1Seg);
    testMap.insert(0.0, model2Seg);
    ASSERT_EQ(testMap.modelCount, 2);
   }


//Test breakpoint insertion based on panel 2 (Middle with three models. Low start loss for #3, 2 not considered.)
TEST(InsertTests, testInsertMiddle)
   {
    ModelSelectionMap testMap = ModelSelectionMap();
    Model model1Seg = Model(1, 7.0);
    Model model2Seg = Model(2, 4.0); 
    Model model3Seg = Model(3, 0.0);
    testMap.insert(4.0, model1Seg);
    testMap.insert(3.0, model2Seg); //?????? Never selected so how should I test this? Maybe insert 3 segments after and test the update?
    //Test for path here? Form link list?
    testMap.insert(1.0, model3Seg); //Add model with 3 segments that is found to be more optimal
    //Should I remove the 2 segment model from the data structure now?
    ASSERT_EQ(testMap.modelCount, 3);
   }



//Test breakpoint insertion based on panel 2 (Right with three models. Higher start loss for #3, all models considered on path.)
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