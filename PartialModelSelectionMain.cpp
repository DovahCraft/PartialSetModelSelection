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

//Testing breakpoint inserts.
TEST(Breakpoints, testBreakFormation)
   {
    Model testModel = Model(2, 3);
    double expectedPen = 4.0;
    Breakpoint testBP = Breakpoint(4.0, testModel);
    ASSERT_EQ(testBP.first, expectedPen);
   }



//Driver function for google test
int main (int argc, char* argv[]){
    //Unit tests here
    
    
    
    minimize(5.44);
    solver(5.34);

    Model model3segs = Model(3, 5);  
    ModelSelectionMap testMap = ModelSelectionMap();
    //Call minimizer
    //Call insert
    testMap.insert(3.5, model3segs);

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}