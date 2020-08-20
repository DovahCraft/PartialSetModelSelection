# PartialSetModelSelection
Implementation of an optimal model selection function using input from a partial set of segmentation models.


Goal is to expand on this [previous linear time algorithm](https://arxiv.org/abs/2003.02808) and complete breakpoint path computation in O(lgn) time using std::map as the core data structure. This new algorithm will be more flexible than the previous one, as we can now select and add to a partial set of segmentation models to consider, rather than all models ε {1..N}. 
