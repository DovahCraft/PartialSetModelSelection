//#pragma once
#include <iostream>
#include <map>

using namespace std;

typedef enum 
	 	{
		 NO_MODEL_CAP,
		 STD_MODEL_CAP=3
		}ModelCapMessages;

typedef pair<double, Model> breakPoint;

class Model {
	public:
		//Number of segments (k-value)
		int model_size;
		//Loss associated with the given model 
		double loss;
		
		//Init constructor declaration
		Model(int model_size, double loss) 
			{
			 setModelSize(model_size);
			}

		//Setter method to set a new model size.
		void setModelSize(int model_size) {
			this->model_size = model_size;
		}
		int getModelSize(int model_size) {
			return this->model_size;
		}
		
		//Getters and setters for model_loss value.
		void setModelLoss(double model_loss) {
			this->model_size = model_size;
		}

		void getModelLoss(double model_loss) {
			this->loss = model_size;
		}
};
	
class ModelSelectionMap {
	public:
		std::map<double, Model> breakpoints;
		ModelCapMessages maxModels;

    ModelSelectionMap() 
	   {
		maxModels = NO_MODEL_CAP;
	   }


	ModelSelectionMap(ModelCapMessages cap)
	   {   
		maxModels = cap;
	   }

	void insert(double penalty, Model currentModel)
	   {
		cout<<"Calling insert on breakpoint map\n";    
	   }
};

//Class to embody Model,Boolean pairs for model selection.
class MinimizeResult {
	public:
		int model_size;
		bool certain;
};




//Function definitions
void insert(double penalty, Model currentModel, ModelSelectionMap currentMap);
tuple<int, bool> minimize(double penalty);
tuple<int, int> solver(double penalty);
