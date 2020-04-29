#include <iostream>
using namespace std;
#include <map>
#include "PartialModelSelection.h"
/*
Function name: Main
Algorithm/Use: Temporarily used for testing of the functions below.
*/
int main(void)
{
   //Unit tests here
    cout << "Hello world!\n";
    insert(3.5, 3, 5);
    minimize(5.44);
    solver(5.34);
    Model model3segs = Model(3, 5);  
    ModelSelectionMap firstMap = ModelSelectionMap();
}


/*
Function name: insert
Algorithm: Inserts a new model into our partial set (map) data structure
Precondition: The model is formatted correctly and the
   penalty is a valid float?
Postcondition: Inserts the model into the data structure.
   No return type, maybe return success code eventually.
Exceptions: correctly and appropriately (without program failure)
    responds to and reports failure to insert the model.
Note: none
*/
void insert(double penalty, Model currentModel, ModelSelectionMap currentMap)
   {
    currentMap.insert(penalty,currentModel);
    cout << "Calling insertsdfasdfdsvf! \n";
   }

/*
Function name: Minimize
Algorithm: Acquires a penalty value lambda and returns a tuple consisting of:
   (the k value corresponding to the selected model, a boolean showing its accuracy)
   Boolean values consist of: F for unsure, and T for a sure pairing.
Precondition: for correct operation, the passed penalty is a valid
   float value.
Postcondition: in correct operation, computes what model is optimal
   for the passed penalty, and gives a boolean signifying its accuracy.
   Both these values are passed back as a tuple.
Exceptions: correctly and appropriately (without program failure)
    responds to and reports incorrectly formatted op code data,
Note: none
*/
tuple<int, bool> minimize(double penalty)
   {
    cout << "Testing minimize\n";
    tuple<int, bool> tempTuple = make_tuple(4, true);
    //Temporary stub return
    return tempTuple;
   }

/*
Function name: Solver
Algorithm:

Precondition: for correct operation, the passed penalty is a valid
   float value.
Postcondition:
Exceptions: correctly and appropriately (without program failure)
    responds to and reports incorrectly formatted op code data,
Note: none
*/
tuple<int, int> solver(double penalty)
   {
    tuple<int, bool> tempTuple = make_tuple(4, true);
    cout << "Testing solver!\n";
    //Temporary stub return
    return tempTuple;
   }
