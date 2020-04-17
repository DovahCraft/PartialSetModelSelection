#pragma once

class Model {
	public:
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

	private:
		//Number of segments (k-value)
		int model_size;
		//Loss associated with the given model 
		double loss;

};
	
class ModelSelectionMap {
	public:
		std::map<double, Model> breakpoints;

    ModelSelectionMap() 
	   {
		cout << "Making map";
	   }
};

//Class to embody Model,Boolean pairs for model selection.
class MinimizeResult {
	public:
		int model_size;
		bool certain;
};

//Function definitions
void insert(double penalty, int model, int loss);
tuple<int, bool> minimize(double penalty);
tuple<int, int> solver(double penalty);